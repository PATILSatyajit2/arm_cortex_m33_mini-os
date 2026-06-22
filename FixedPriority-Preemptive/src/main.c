#include "os.h"

/******************************************************************************/

uint32 Task1_EntryCounter;;
uint32 Task2_EntryCounter;;
uint32 Task1_Counter;
uint32 Task2_Counter;

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

int main(void)
{
  Os_Init();
  Task1();
  while(1)
  {
  }
  
  return 0;
}
