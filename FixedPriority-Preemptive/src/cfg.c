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

/* Static Structure - Kast_DynamicTaskInfoType (Used as runtime storage */
DynamicTaskInfoType DynamicTaskInfo[OS_TASK_COUNT] =
{
  {
    /* Default Task State : DNM */
    READY, 
    
    /* Current Activation Count : DNM */
    0
  },
  {
    /* Default Task State : DNM */
    READY, 
    
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
    
    /* Task ID */
    TASK_0,
    
    /* Task Priority */
    0,
    
    /* Task Activation Count */
    2
  },
  {
    /* Dynamic Task Info Pointer */
    &DynamicTaskInfo[1],
    
    &ContextInfo[1],
    
    /* Task ID */
    TASK_1,
    
    /* Task Priority */
    1,
    
    /* Task Activation Count */
    2
  }
};
