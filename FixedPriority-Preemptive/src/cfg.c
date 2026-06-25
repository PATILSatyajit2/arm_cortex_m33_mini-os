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
    0
  },
  /* Priority Level 1 */
  {
    /* Head Pointer */
    NULL_PTR,
    
    /* Next Task In Queue Pointer */
    &Os_NextTaskInQueue[2],
    
    /* Read index */
    0,
    
    /* Write index */
    0
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
  }
};

/* Static Structure - Kast_DynamicTaskInfoType (Used as runtime storage */
DynamicTaskInfoType DynamicTaskInfo[OS_TASK_COUNT] =
{
  {
    /* Default Task State : DNM */
    READY, 
    
    /* Task ID */
    0,
    
    /* Current Activation Count : DNM */
    0,
  },
  {
    /* Default Task State : DNM */
    READY, 
    
    /* Task ID */
    1,
    
    /* Current Activation Count : DNM */
    0
  }
};

/* Static Structure - Kast_StaticTaskInfo (Holds configuration info) */
const StaticTaskInfoType StaticTaskInfo[OS_TASK_COUNT] = 
{
  {
    /* Dynamic Task Info Pointer */
    &DynamicTaskInfo[0],
    
    &ContextInfo[0],
    
    Task1,
    
    /* Task ID */
    TASK_0,
    
    /* Stack index */
    0,
    
    /* Task Priority */
    1,
    
    /* Task Activation Count */
    2
  },
  {
    /* Dynamic Task Info Pointer */
    &DynamicTaskInfo[1],
    
    &ContextInfo[1],
    
    NULL_PTR,
    
    /* Task ID */
    TASK_1,
    
    /* Stack index */
    1,
    
    /* Task Priority */
    2,
    
    /* Task Activation Count */
    2
  }
};
