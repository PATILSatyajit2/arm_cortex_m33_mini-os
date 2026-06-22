#ifndef PORT_H
#define PORT_H

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

#endif

