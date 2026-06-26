// #ifndef CFG_H
// #define CFG_H

// #include "global_definations.h"
#include "os.h"

/* Count of unique task priorities */
#define OS_UNIQUE_PRIORITIES      3u

/* Count of all individual task activation count */
#define OS_MAX_TASK_ACTIVATION    5u

/* Total task count */
#define OS_TASK_COUNT             3u

/* Macro for Idle Task identification */
#define IDLE_TASK                 0u

/* Macro for Task0 identification */
#define TASK_0                    1u

/* Macro for Task1 identification */
#define TASK_1                    2u

#define OS_STACK_SIZE             192u

/* Autostart task count */
#define OS_AUTOSTART_TASK_COUNT   2u

/******************************************************************************/
extern void Task1();
extern void Task2();

// #endif
