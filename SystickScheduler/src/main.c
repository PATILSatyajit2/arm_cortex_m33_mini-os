#include <stdio.h>
#include "port.h"

/******************************************************************************/
#define OS_TASK_COUNT                   2u
#define OS_STACK_SIZE                   64u
/******************************************************************************/

typedef unsigned char uint8;
typedef unsigned int uint32;

typedef struct {
    uint32 Register_R5;
    uint32 Register_R6;
    uint32 Register_R7;
    uint32 Register_R8;
    uint32 Register_R9;
    uint32 Register_R10;
    uint32 Register_R11;
    uint32 Register_R13;
} Context_TypeInfo;

typedef struct {
    uint32      stack[OS_STACK_SIZE];     /* private stack */
    Context_TypeInfo      context;     /* private stack */
    uint32      *sp;                       /* offset 0 — current stack pointer */
    void (*task_func)(void);
    uint8       state;                    /* offset 4 */
    uint8       id;                       /* offset 8 */
} TCB_t;

/******************************************************************************/

uint32 Os_Systick_Counter;
uint32 Os_task_counter;
uint32 Os_Running_Task;

uint32 Task1_EntryCounter;;
uint32 Task2_EntryCounter;;
uint32 Task1_Counter;
uint32 Task2_Counter;

TCB_t Os_StaticTasks[OS_TASK_COUNT];
/******************************************************************************/

void Os_Systick_Init(void);
void Os_Systick_Handler(void);
void Os_Schedule(void);
void Os_UpdateR4(uint32 val);
void Os_Task_CSAInit(uint32* Stack_frame, void (*task_func)(void));

/******************************************************************************/

void Os_Systick_Init(void)
{
  OS_SYSTICK_CSR = 0x0;
  OS_SYSTICK_RVR = OS_SYSTICK_1s - 1;
  OS_SYSTICK_CSR = 0x7u;
}

__attribute((__naked__)) void Os_Systick_Handler(void)
{
  __asm volatile ( " stmia  r4, {r5-r11}            \n"
                   " mrs    r0, msp                 \n"
                   " str    r0, [r4, #28]           \n"
                   " BL     Os_Schedule             \n"
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

void Os_Schedule(void)
{
  Os_task_counter = (Os_task_counter + 1)%(OS_TASK_COUNT);
  
  TCB_t* Lpt_tcb = &Os_StaticTasks[Os_task_counter];
  
  uint32* Stack_frame = &Lpt_tcb->stack[OS_STACK_SIZE-1];
  
  Os_UpdateR4((uint32)&Os_StaticTasks[Os_task_counter].context);
}

void Os_Task_CSAInit(uint32* Stack_frame, void (*task_func)(void))
{  Stack_frame = Stack_frame - 8;
  Stack_frame[0] = 0;
  Stack_frame[1] = 1;
  Stack_frame[2] = 2;
  Stack_frame[3] = 3;
  Stack_frame[4] = 12;
  Stack_frame[5] = 0x0;                     /* LR */
  Stack_frame[6] = (uint32)task_func;       /* PC */
  Stack_frame[7] = 0x01000000;              /* xPSR */

  Os_StaticTasks[Os_task_counter].context.Register_R13 =
      (uint32)Stack_frame;
}

void Os_Task_Create(uint8 task_id, void (*task_func)(void))
{
  if(OS_TASK_COUNT < Os_task_counter) return;
  
  TCB_t* Lpt_tcb = &Os_StaticTasks[Os_task_counter];
  
  Lpt_tcb->id = task_id;
  Lpt_tcb->task_func = task_func;
  
  uint32* Stack_frame = &Lpt_tcb->stack[OS_STACK_SIZE-1];
  Stack_frame = (uint32*)(((uint32)Stack_frame) & (~0x07));
  Os_Task_CSAInit(Stack_frame, task_func);
  
  Lpt_tcb->sp = Stack_frame;
  
  Os_task_counter = (Os_task_counter + 1)%(OS_TASK_COUNT);
}

/******************************************************************************/
void Task1()
{
  ++Task1_EntryCounter;
  while(1)
  {
    ++Task1_Counter;
  }
}

void Task2()
{
  ++Task2_EntryCounter;
  while(1)
  {
    ++Task2_Counter;
  }
}

void Os_Init(void)
{
  OS_M_DISABLEFPU();
  
  /* Initialize Os dynamic variables */
  Os_UpdateR4((uint32)&Os_StaticTasks[Os_task_counter].context);
  
  /* Initialize Systick */
  Os_Systick_Init();
}

int main(void)
{
  Os_Init();
  
  Os_Task_Create(0, Task1);
  Os_Task_Create(1, Task2);
  
  Task1();
  while(1)
  {
  }
  
  return 0;
}
