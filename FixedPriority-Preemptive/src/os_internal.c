#include "os.h"

/******************************************************************************/
Os_StatusType Os_internal_enqueTaskInQueue(StaticTaskInfoType* ptr_StaticTaskInfo);
Os_StatusType Os_internal_dequeTaskFromQueue(StaticTaskInfoType* ptr_StaticTaskInfo);
void Os_TerminateTask(uint8 taskId);
void Os_ActivateTask(uint8 taskId);
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

__attribute((__naked__)) void Os_Systick_Handler(void)
{
  __asm volatile ( " stmia  r4, {r5-r11}            \n"
                   " mrs    r0, msp                 \n"
                   " str    r0, [r4, #28]           \n"
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

void Os_Task_CSAInit(uint32* Stack_frame, void (*task_func)(void))
{  // Stack_frame = Stack_frame - 8;
  // Stack_frame[0] = 0;
  // Stack_frame[1] = 1;
  // Stack_frame[2] = 2;
  // Stack_frame[3] = 3;
  // Stack_frame[4] = 12;
  // Stack_frame[5] = (uint32)&Os_TerminateTask;                     /* LR */
  // Stack_frame[6] = (uint32)task_func;       /* PC */
  // Stack_frame[7] = 0x01000000;              /* xPSR */

  // Os_StaticTasks[Os_task_counter].context.Register_R13 =
      // (uint32)Stack_frame;
}

/******************************************************************************/

void Os_Init(void)
{
  OS_M_DISABLEFPU();
  
  /* Initialize Os dynamic variables */
  Os_UpdateR4((uint32)&StaticTaskInfo[ZERO].context);
  
  /* Initialize Systick */
  Os_Systick_Init();
}

Os_StatusType Os_internal_enqueTaskInQueue(StaticTaskInfoType* ptr_StaticTaskInfo)
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
    ptr_StaticPriorityQueueInfo->head = &(ptr_StaticTaskInfo->dynamicTaskInfo);
    
    /* Set global priority level */
    Os_CurrentPriority |= priority;
  }
  
  /* TODO - below need to be updated ---> get a static parameter for roll over per priority */
  writeIndex = (writeIndex + ONE)%2;
  
  return ret;
}

Os_StatusType Os_internal_dequeTaskFromQueue(StaticTaskInfoType* ptr_StaticTaskInfo)
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
  
  /* TODO - below need to be updated ---> get a static parameter for roll over per priority */
  readIndex = (readIndex + ONE)%2;
  
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

Os_StatusType Os_internal_TaskDisapatcher(void)
{
  StaticTaskInfoType* ptr_StaticTaskInfo;
  DynamicTaskInfoType* ptr_DynamicTaskInfo;
  
  /* Priority queue local pointer */
  StaticPriorityQueueInfoType* ptr_StaticPriorityQueueInfo;
  
  /* Local variable for priority */
  uint8 priority;
  
  /* Return status */
  Os_StatusType ret;
  
  ret = OK;
  
  /* Take local pointer to priority queue of requested task */
  ptr_StaticPriorityQueueInfo = &StaticPriorityQueueInfo[priority];
  
  if( (priority > Os_CurrentPriority) &&
     NULL_PTR != ptr_StaticPriorityQueueInfo->head )
  {
    ptr_DynamicTaskInfo = *ptr_StaticPriorityQueueInfo->head;
  }
}

void Os_ActivateTask(uint8 taskId)
{
  StaticTaskInfoType* ptr_StaticTaskInfo;
  DynamicTaskInfoType* ptr_DynamicTaskInfo;
  
  /* Return status */
  Os_StatusType ret;
  
  ret = OK;
  
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
    if(OK == Os_internal_enqueTaskInQueue(ptr_StaticTaskInfo))
    {
      
    }
  }
  else
  {
    return;
  }
}

void Os_TerminateTask(uint8 taskId)
{
  Os_internal_dequeTaskFromQueue(&StaticTaskInfo[taskId]);
}
