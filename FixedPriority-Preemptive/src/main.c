#include "os.h"

/******************************************************************************/

uint32 Task1_EntryCounter;;
uint32 Task2_EntryCounter;;
uint32 Task1_Counter;
uint32 Task2_Counter;

/******************************************************************************/
void Task1();

void Task1()
{
  volatile uint8 u8_var1;
  volatile uint32 u32_var2;
  volatile uint32 u32_arr[10];

  u8_var1 = 10;
  u32_var2 = 1020;

  for(int i=0;i<10;i++)
  {
	  u32_arr[i] = i*i;
  }

  Task1_EntryCounter = u32_var2 + u8_var1;

  Os_ActivateTask(2);

  Task1_Counter = u32_var2 + u8_var1;

  for(int i=0;i<10;i++)
  {
	  if(u32_arr[i] != i*i)
	  {
		  Task1_Counter = 0xFF;
	  }
  }
}

void Task2()
{
  ++Task2_Counter;
  Os_TerminateTask();
}

int main(void)
{
  Os_Init();
  // Task1();
  while(1)
  {
  }
  
  return 0;
}
