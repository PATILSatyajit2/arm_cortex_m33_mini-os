#ifndef OS_H
#define OS_H

#include "global_definations.h"
#include "port.h"
#include "cfg.h"

#define OS_SYSTICK_CSR                  (*(volatile uint32*)0xE000E010u)
#define OS_SYSTICK_RVR                  (*(volatile uint32*)0xE000E014u)
#define OS_SYSTICK_CVR                  (*(volatile uint32*)0xE000E018u)
#define OS_CPACR                        (*(volatile uint32*)0xE000ED88u)

#define OS_SYSTICK_1ms                  12000
#define OS_SYSTICK_100ms                OS_SYSTICK_1ms*100
#define OS_SYSTICK_1s                   OS_SYSTICK_1ms*1000
#define OS_SYSTICK_5s                   OS_SYSTICK_1ms*5000
#define OS_SYSTICK_10s                  OS_SYSTICK_1ms*10000

#define OS_M_KERNEL_PANIC()             do{}while(1);

#define OS_M_UPDATEGLOBALCONTEXT(ptr)   do{\
                                          Os_GlobalContext = ptr;\
                                        }while(0);

#define OS_M_SWITCH_TASK_CONTEXT()      do{\
                                          Os_internal_TaskDispatcher();\
                                          __asm("svc #0");\
                                        }while (0);

#define OS_M_TASK_PUSH_TO_QUEUE(ptr_StaticTaskInfo)   \
                                        do{\
                                            Os_internal_enqueTaskInQueue(ptr_StaticTaskInfo);\
                                            ptr_StaticTaskInfo->dynamicTaskInfo->currentActivationCount += ONE;\
                                            ptr_StaticTaskInfo->dynamicTaskInfo->taskState = READY_AND_INIT;\
                                          }while(0);
                                                  
/******************************************************************************/

// typedef stag_StaticISRInfo StaticISRInfo;

typedef struct
{
  t_taskState taskState;
  t_taskID taskID;
  uint8 currentActivationCount;
}DynamicTaskInfoType;

typedef struct
{
  uint32* startAdd;
  uint32* endAdd;
}StaticStackInfoType;

typedef struct
{
  DynamicTaskInfoType* dynamicTaskInfo;
  Context_TypeInfo* context;
  void (*task_func)(void);
  t_taskID taskID;
  uint8 stackIndex;
  uint8 priority;
  uint8 maxActivation;
}StaticTaskInfoType;

typedef struct 
{
  DynamicTaskInfoType** head;
  DynamicTaskInfoType** next;
  uint8 readIndex;
  uint8 writeIndex;
  uint8 maxActivation;
}StaticPriorityQueueInfoType;

/******************************************************************************/
extern uint8 Os_CurrentPriority;
extern Context_TypeInfo* Os_GlobalContext;
/******************************************************************************/

extern void Os_Init(void);
extern void Os_idleTask(void);
extern void Os_Systick_Init(void);
extern void Os_Systick_Handler(void);
extern void Os_PendSV_Handler(void);
extern void Os_UpdateR4(uint32 val);
extern void Os_Task_CSAInit(uint32* Stack_frame, void (*task_func)(void));
extern Os_StatusType Os_ActivateTask(uint8 taskId);
extern void Os_TerminateTask(void);

/******************************************************************************/

/* Extern declaration of DynamicTaskInfoType to hold next task info */
extern DynamicTaskInfoType* Os_NextTaskInQueue[OS_MAX_TASK_ACTIVATION];
extern DynamicTaskInfoType DynamicTaskInfo[OS_TASK_COUNT] ;
extern StaticPriorityQueueInfoType StaticPriorityQueueInfo[OS_UNIQUE_PRIORITIES];
extern const StaticTaskInfoType StaticTaskInfo[OS_TASK_COUNT];
extern const StaticStackInfoType StaticStackInfo[OS_TASK_COUNT];
extern const uint8 StaticAutostartTaskInfo[OS_AUTOSTART_TASK_COUNT];
/******************************************************************************/
#endif

