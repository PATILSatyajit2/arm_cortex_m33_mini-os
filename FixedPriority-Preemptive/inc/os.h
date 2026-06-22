#ifndef OS_H
#define OS_H

#include "global_definations.h"
#include "port.h"
#include "cfg.h"

#define OS_SYSTICK_CSR    (*(volatile uint32*)0xE000E010u)
#define OS_SYSTICK_RVR    (*(volatile uint32*)0xE000E014u)
#define OS_SYSTICK_CVR    (*(volatile uint32*)0xE000E018u)
#define OS_CPACR          (*(volatile uint32*)0xE000ED88u)

#define OS_SYSTICK_1ms          12000
#define OS_SYSTICK_100ms        OS_SYSTICK_1ms*100
#define OS_SYSTICK_1s           OS_SYSTICK_1ms*1000
#define OS_SYSTICK_5s           OS_SYSTICK_1ms*5000
#define OS_SYSTICK_10s           OS_SYSTICK_1ms*10000

#define OS_M_DISABLEFPU()   do{\
                                OS_CPACR = 0x0;\
                            }while(0);

/******************************************************************************/

// typedef stag_StaticISRInfo StaticISRInfo;

typedef struct
{
  t_taskState taskState;
  uint8 currentActivationCount;
}DynamicTaskInfoType;

typedef struct
{
  DynamicTaskInfoType* dynamicTaskInfo;
  Context_TypeInfo* context;
  t_taskID taskID;
  uint8 priority;
  uint8 maxActivation;
}StaticTaskInfoType;

typedef struct 
{
  DynamicTaskInfoType** head;
  DynamicTaskInfoType** next;
  uint8 readIndex;
  uint8 writeIndex;
}StaticPriorityQueueInfoType;

/******************************************************************************/
extern uint8 Os_CurrentPriority;
DynamicTaskInfoType* Os_RunningDynamicTaskInfo;
/******************************************************************************/

extern void Os_Init(void);
extern void Os_Systick_Init(void);
extern void Os_Systick_Handler(void);
extern void Os_UpdateR4(uint32 val);
extern void Os_Task_CSAInit(uint32* Stack_frame, void (*task_func)(void));
extern void Os_ActivateTask(uint8 taskId);

/******************************************************************************/

/* Extern declaration of DynamicTaskInfoType to hold next task info */
extern DynamicTaskInfoType* Os_NextTaskInQueue[OS_MAX_TASK_ACTIVATION];
extern StaticPriorityQueueInfoType StaticPriorityQueueInfo[OS_UNIQUE_PRIORITIES];
extern DynamicTaskInfoType DynamicTaskInfo[OS_TASK_COUNT] ;
extern const StaticTaskInfoType StaticTaskInfo[OS_TASK_COUNT];

/******************************************************************************/
#endif

