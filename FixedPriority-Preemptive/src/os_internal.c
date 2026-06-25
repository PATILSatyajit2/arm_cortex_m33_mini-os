#include "os.h"

/******************************************************************************/
Os_StatusType Os_internal_enqueTaskInQueue(const StaticTaskInfoType* ptr_StaticTaskInfo);
Os_StatusType Os_internal_dequeTaskFromQueue(const StaticTaskInfoType* ptr_StaticTaskInfo);
Os_StatusType Os_internal_TaskDispatcher(void);
void Os_TerminateTask(uint8 taskId);

DynamicTaskInfoType* Os_RunningDynamicTaskInfo;
Context_TypeInfo* Os_GlobalContext;
uint8 Os_CurrentPriority;
static uint8 Os_CallLevel;

/******************************************************************************/

#define OS_SYSTICK_DISABLE                    0x0u
#define OS_SYSTICK_EN                         0x1u
#define OS_SYSTICK_EXC_EN                     0x2u
#define OS_SYSTICK_PROCESSOR_CLK              0x4u

void Os_Systick_Init(void)
{
  OS_SYSTICK_CSR = OS_SYSTICK_DISABLE;
  
  OS_SYSTICK_RVR = OS_SYSTICK_1s - ONE;
  
  OS_SYSTICK_CSR = OS_SYSTICK_EN | 
                   OS_SYSTICK_EXC_EN | 
                   OS_SYSTICK_PROCESSOR_CLK;
}

__attribute((__naked__)) void Os_PendSV_Handler(void)
{
  __asm volatile ( " ldr    r0, =Os_CallLevel       \n"
                   " ldr    r0, [r0]                \n"
                   " cmp    r0, #0                  \n"
                   " beq    L_skilStore             \n"
                   " stmia  r4, {r5-r11}            \n"
                   " mrs    r0, msp                 \n"
                   " str    r0, [r4, #28]           \n"
                   "L_skilStore:                    \n"
                   " ldr    r0, =Os_GlobalContext   \n"
                   " ldr    r4, [r0]                \n"
                   " ldmia  r4, {r5-r11}            \n"
                   " ldr    r0, [r4, #28]           \n"
                   " msr    msp, r0                 \n"
                   " ldr    lr, =0xFFFFFFF9         \n"
                   " bx     lr                        ");
}

__attribute((__naked__)) void Os_Systick_Handler(void)
{
  __asm volatile ( " stmia  r4, {r5-r11}            \n"
                   " mrs    r0, msp                 \n"
                   " str    r0, [r4, #28]           \n"
                   " mov    r0, #0                  \n"
                   " bl     Os_ActivateTask         \n"
                   " ldmia  r4, {r5-r11}            \n"
                   " ldr    r0, [r4, #28]           \n"
                   " msr    msp, r0                 \n"
                   " ldr    lr, =0xFFFFFFF9         \n"
                   " bx     lr                        ");
}

__attribute((__naked__)) void Os_UpdateR4(uint32 val)
{
  __asm volatile(" mov r4, r0 \n"
                 " bx lr ");
}

uint32 Os_CheckLeadingZeros(uint32 val)
{
    uint32 result;

    __asm volatile(
        "clz %0, %1"
        : "=r"(result)
        : "r"(val));

    return 32U - result;
}

void Os_internal_Task_CSAInit(const StaticTaskInfoType* ptr_StaticTaskInfo)
{
  StackFrame_Type* Stack_frame;
  const StaticStackInfoType* StackInfo;
  StackInfo = &StaticStackInfo[ptr_StaticTaskInfo->stackIndex];
  
  Stack_frame = (StackFrame_Type*)(StackInfo->endAdd - 8);
  
  Stack_frame->Register_R0 = 0;
  Stack_frame->Register_R1 = 1;
  Stack_frame->Register_R2 = 2;
  Stack_frame->Register_R3 = 3;
  Stack_frame->Register_R12 = 12;
  Stack_frame->Register_R14 = (uint32)&Os_TerminateTask;                    /* LR */
  Stack_frame->Register_R15 = (uint32)ptr_StaticTaskInfo->task_func;        /* PC */
  Stack_frame->Register_XPSR = 0x01000000;                                  /* xPSR */

  ptr_StaticTaskInfo->context->Register_R13 =
      (uint32)Stack_frame;
}

/******************************************************************************/

void Os_Init(void)
{
  OS_M_DISABLEFPU();
  
  /* Initialize Os dynamic variables */
  Os_UpdateR4((uint32)StaticTaskInfo[ZERO].context);
  
  /* Initialize Systick */
  // Os_Systick_Init();
  
  Os_CallLevel = 0;
  Os_ActivateTask(0);
}

Os_StatusType Os_internal_enqueTaskInQueue(const StaticTaskInfoType* ptr_StaticTaskInfo)
{
  uint8 priority;
  StaticPriorityQueueInfoType* ptr_StaticPriorityQueueInfo;
  DynamicTaskInfoType* ptr_DynamicTaskInfo;
  
  /* Local variable for write index */
  uint8 writeIndex;
  
  /* Return status */
  Os_StatusType ret;
  
  ret = OK;
  
  /* Get the priority of task */
  priority = ptr_StaticTaskInfo->priority;
  
  /* Take local pointer to priority queue of requested task */
  ptr_StaticPriorityQueueInfo = &StaticPriorityQueueInfo[priority];
  
  /* Take write index of local priority queue */
  writeIndex = ptr_StaticPriorityQueueInfo->writeIndex;
  
  /* Take pointer to dynamic task info of requested task */
  ptr_DynamicTaskInfo = ptr_StaticTaskInfo->dynamicTaskInfo;
  
  /* Update next task in queue */
  ptr_StaticPriorityQueueInfo->next[writeIndex] = ptr_DynamicTaskInfo;
  
  /* Check if no task is active at requested priority */
  if(NULL_PTR == ptr_StaticPriorityQueueInfo->head)
  {
    /* Update head pointer with dynamic task info of requested task */
    ptr_StaticPriorityQueueInfo->head = &ptr_StaticTaskInfo->dynamicTaskInfo;
    
    /* Set global priority level */
    Os_CurrentPriority |= priority;
  }
 
  /* Write index update */
  ptr_StaticPriorityQueueInfo->writeIndex = (writeIndex + ONE) % 
    ptr_StaticPriorityQueueInfo->maxActivation;
  
  return ret;
}

Os_StatusType Os_internal_dequeTaskFromQueue(const StaticTaskInfoType* ptr_StaticTaskInfo)
{
  /* Priority queue local pointer */
  StaticPriorityQueueInfoType* ptr_StaticPriorityQueueInfo;
  
  /* Local variable for read index */
  uint8 readIndex;
  
  /* Local variable for priority of requested task */
  uint8 priority;
  
  /* Return status */
  Os_StatusType ret;
  
  ret = OK;
  
  /* Get the priority of task */
  priority = ptr_StaticTaskInfo->priority;
  
  /* Take local pointer to priority queue of requested task */
  ptr_StaticPriorityQueueInfo = &StaticPriorityQueueInfo[priority];
  
  readIndex = ptr_StaticPriorityQueueInfo->readIndex;
  
  /* Read index roll over */
  readIndex = (readIndex + ONE) % ptr_StaticPriorityQueueInfo->maxActivation;
  
  ptr_StaticPriorityQueueInfo->readIndex = readIndex;
  
  if(NULL_PTR != ptr_StaticPriorityQueueInfo->next[readIndex])
  {
    /* Update head pointer */
    ptr_StaticPriorityQueueInfo->head = &(ptr_StaticPriorityQueueInfo->next[readIndex]);
  }
  else
  {
    /* Update head as NULL_PTR */
    ptr_StaticPriorityQueueInfo->head = NULL_PTR;
    
    /* Clear the global priority */
    Os_CurrentPriority |= priority;
  }
  
  return ret;
}

void Os_internal_updateQueue(void)
{
  /* Priority queue local pointer */
  StaticPriorityQueueInfoType* ptr_StaticPriorityQueueInfo;
  
  uint8 priority;
  uint8 readIndex;
  
  /* Get highest priority */
  priority = OS_M_GETHIGHESTPRIORITY();
  
  /* Take local pointer to priority queue of requested task */
  ptr_StaticPriorityQueueInfo = &StaticPriorityQueueInfo[priority];
  
  readIndex = ptr_StaticPriorityQueueInfo->readIndex;
  
  if(NULL_PTR != ptr_StaticPriorityQueueInfo->next[readIndex])
  {
    /* Current priority queue head pointer advancement */
    ptr_StaticPriorityQueueInfo->head = 
      &ptr_StaticPriorityQueueInfo->next[readIndex];
      
    /* TODO - below need to be updated ---> get a static parameter for roll over per priority */
    readIndex = (readIndex + ONE)%2;
    
    /* Update the readIndex */
    ptr_StaticPriorityQueueInfo->readIndex = readIndex;
  }
  else
  {
    /* Global priority bit mask clear */
    Os_CurrentPriority |= priority;
  }
}

Os_StatusType Os_internal_TaskDispatcher(void)
{
  const StaticTaskInfoType* ptr_StaticTaskInfo;
  DynamicTaskInfoType* ptr_DynamicTaskInfo;
  
  /* Priority queue local pointer */
  StaticPriorityQueueInfoType* ptr_StaticPriorityQueueInfo;
  
  /* Local variable for priority */
  uint8 priority;
  
  /* Return status */
  Os_StatusType ret;
  
  ret = SAVE_AND_LOAD_CONTEXT;
  
  priority = OS_M_GETHIGHESTPRIORITY();
  
  /* Take local pointer to priority queue of requested task */
  ptr_StaticPriorityQueueInfo = &StaticPriorityQueueInfo[priority];
  
  ptr_DynamicTaskInfo = *ptr_StaticPriorityQueueInfo->head;
  
  /* Take pointer to static task info */
  ptr_StaticTaskInfo = &StaticTaskInfo[ptr_DynamicTaskInfo->taskID];
  
  /* Initialize default context */
  Os_internal_Task_CSAInit(ptr_StaticTaskInfo);
  
  /* Update task state */
  ptr_DynamicTaskInfo->taskState = RUNNING;
  
  /* Update Running Task */
  Os_RunningDynamicTaskInfo = ptr_DynamicTaskInfo;
  
  /* Update Global Context */
  OS_M_UPDATEGLOBALCONTEXT(ptr_StaticTaskInfo->context);
  
  return ret;
}

Os_StatusType Os_ActivateTask(uint8 taskId)
{
  const StaticTaskInfoType* ptr_StaticTaskInfo;
  DynamicTaskInfoType* ptr_DynamicTaskInfo;
  
  /* Return status */
  Os_StatusType ret;
  
  ret = N_OK;
  
  /* Take local pointer to static task info */
  ptr_StaticTaskInfo = &StaticTaskInfo[taskId];
  
  /* Take local pointer to dynamic task info */
  ptr_DynamicTaskInfo = ptr_StaticTaskInfo->dynamicTaskInfo;
  
  /* Check for valid taskId */
  
  /* Check activation count */
  if(ptr_DynamicTaskInfo->currentActivationCount <
      ptr_StaticTaskInfo->maxActivation)
  {
    /* Insert Task in Priority Queue */
    ret = Os_internal_enqueTaskInQueue(ptr_StaticTaskInfo);
    
    if(OK == ret)
    {
      /* Call task dispatcher */
      Os_internal_TaskDispatcher();
      
      /* Invoke pendsv */
      OS_M_SYSCALL_PENDSV();
      __asm("svc #0");
    }
  }
  
  return ret;
}

void Os_TerminateTask(uint8 taskId)
{
  const StaticTaskInfoType* ptr_StaticTaskInfo;
  DynamicTaskInfoType* ptr_DynamicTaskInfo;
  
  /* Take pointer to static task info */
  ptr_StaticTaskInfo = &StaticTaskInfo[taskId];
  
  /* Take pointer to dynamic task info */
  ptr_DynamicTaskInfo = ptr_StaticTaskInfo->dynamicTaskInfo;;
  
  /* Update Task state */
  ptr_DynamicTaskInfo->taskState = SUSPENDED;
  
  /* Deque task from queue */
  Os_internal_dequeTaskFromQueue(ptr_StaticTaskInfo);
  
  /* Update the queue */
  Os_internal_updateQueue();
  
  /* Dispatch task */
  Os_internal_TaskDispatcher();
  
  /* Invoke pendsv */
  OS_M_SYSCALL_PENDSV();
  
  /* Execution should not come here */
  OS_M_KERNEL_PANIC();
}
