#include "cfg.h"

Context_TypeInfo ContextInfo[OS_TASK_COUNT];

StaticPriorityQueueInfoType StaticPriorityQueueInfo[OS_UNIQUE_PRIORITIES] = 
{
  /* Priority Level 0 */
  {
    /* Head Pointer */
    NULL_PTR,
    
    /* Next Task In Queue Pointer */
    &Os_NextTaskInQueue[0],
    
    /* Read index */
    0,
    
    /* Write index */
    0,

    /* Count of total tasks activations */
    1
  },
  /* Priority Level 1 */
  {
    /* Head Pointer */
    NULL_PTR,
    
    /* Next Task In Queue Pointer */
    &Os_NextTaskInQueue[1],
    
    /* Read index */
    0,
    
    /* Write index */
    0,

    /* Count of total tasks activations */
    2
  },
  /* Priority Level 2 */
  {
    /* Head Pointer */
    NULL_PTR,
    
    /* Next Task In Queue Pointer */
    &Os_NextTaskInQueue[3],
    
    /* Read index */
    0,
    
    /* Write index */
    0,

    /* Count of total tasks activations */
    2
  },
};

/* Array of type DynamicTaskInfoType to hold next task info */
DynamicTaskInfoType* Os_NextTaskInQueue[OS_MAX_TASK_ACTIVATION];

uint32 stack[OS_STACK_SIZE];

const StaticStackInfoType StaticStackInfo[OS_TASK_COUNT] =
{
  {
    &stack[0], &stack[63]
  },
  {
    &stack[64], &stack[127]
  },
  {
    &stack[128], &stack[191]
  }  
};

/* Static Structure - Kast_DynamicTaskInfoType (Used as runtime storage */
DynamicTaskInfoType DynamicTaskInfo[OS_TASK_COUNT] =
{
  {
    /* Default Task State : DNM */
    SUSPENDED, 
    
    /* Task ID */
    0,
    
    /* Current Activation Count : DNM */
    0,
  },
  {
    /* Default Task State : DNM */
    SUSPENDED, 
    
    /* Task ID */
    1,
    
    /* Current Activation Count : DNM */
    0,
  },
  {
    /* Default Task State : DNM */
    SUSPENDED, 
    
    /* Task ID */
    2,
    
    /* Current Activation Count : DNM */
    0
  }
};

/* Static Structure - Kast_StaticTaskInfo (Holds configuration info) */
const StaticTaskInfoType StaticTaskInfo[OS_TASK_COUNT] = 
{
  {
    /* Dynamic Task Info Pointer */
    &DynamicTaskInfo[IDLE_TASK],
    
    &ContextInfo[IDLE_TASK],
    
    Os_idleTask,
    
    /* Task ID */
    IDLE_TASK,
    
    /* Stack index */
    0,
    
    /* Task Priority */
    0,
    
    /* Task Activation Count */
    1
  },
  {
    /* Dynamic Task Info Pointer */
    &DynamicTaskInfo[TASK_0],
    
    &ContextInfo[TASK_0],
    
    Task1,
    
    /* Task ID */
    TASK_0,
    
    /* Stack index */
    1,
    
    /* Task Priority */
    1,
    
    /* Task Activation Count */
    2
  },
  {
    /* Dynamic Task Info Pointer */
    &DynamicTaskInfo[TASK_1],
    
    &ContextInfo[TASK_1],
    
    Task2,
    
    /* Task ID */
    TASK_1,
    
    /* Stack index */
    2,
    
    /* Task Priority */
    2,
    
    /* Task Activation Count */
    2
  }
};

/* Autostart Task ID */
const uint8 StaticAutostartTaskInfo[OS_AUTOSTART_TASK_COUNT] = 
{
  /* Autostart task TASK_0 */
  1 ,

  /* Autostart task TASK_1 */
  2
};