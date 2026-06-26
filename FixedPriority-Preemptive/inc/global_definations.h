#ifndef GLOBAL_DEF_H
#define GLOBAL_DEF_H

/******************************************************************************/

#define NULL_PTR                                  ((void *)0)
#define ZERO                                      0u
#define ONE                                       1u

#define OS_SAVE_LOAD_CONTEXT                      1u
#define OS_LOAD_CONTEXT                           0u

/******************************************************************************/

typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;

typedef unsigned char t_taskID;
typedef unsigned char t_priorityLevel;

/******************************************************************************/
typedef enum
{
  SUSPENDED = 0,
  READY,
  READY_AND_INIT,
  WAIT,
  RUNNING
}t_taskState;

typedef enum
{
  OK = 0,
  N_OK,
  SAVE_CONTEXT,
  LOAD_CONTEXT,
  SAVE_AND_LOAD_CONTEXT
}Os_StatusType;

/******************************************************************************/
#endif /* End of GLOBAL_DEF_H */
