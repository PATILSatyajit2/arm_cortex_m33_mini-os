#ifndef PORT_H
#define PORT_H

#define OS_SYSTICK_CSR    (*(volatile uint32*)0xE000E010u)
#define OS_SYSTICK_RVR    (*(volatile uint32*)0xE000E014u)
#define OS_SYSTICK_CVR    (*(volatile uint32*)0xE000E018u)
#define OS_CPACR          (*(volatile uint32*)0xE000ED88u)

#define OS_ICSR           (*(volatile uint32*)0xE000ED88u)

#define OS_PENDSV_SET     0x10000000u

#define OS_SYSTICK_1ms          12000
#define OS_SYSTICK_100ms        OS_SYSTICK_1ms*100
#define OS_SYSTICK_1s           OS_SYSTICK_1ms*1000
#define OS_SYSTICK_5s           OS_SYSTICK_1ms*5000
#define OS_SYSTICK_10s           OS_SYSTICK_1ms*10000

#define OS_M_DISABLEFPU()   do{\
                                OS_CPACR = 0x0;\
                            }while(0);

#define OS_M_GETHIGHESTPRIORITY(val)     Os_CheckLeadingZeros(val)

#define OS_M_SYSCALL_PENDSV()   do{\
                                  __asm("cpsie i");\
                                  OS_ICSR = OS_PENDSV_SET;\
                                }while(0);
/******************************************************************************/

typedef struct {
    uint32 Register_R5;
    uint32 Register_R6;
    uint32 Register_R7;
    uint32 Register_R8;
    uint32 Register_R9;
    uint32 Register_R10;
    uint32 Register_R11;
    uint32 Register_R13;
}Context_TypeInfo;

typedef struct {
    uint32 Register_R0;
    uint32 Register_R1;
    uint32 Register_R2;
    uint32 Register_R3;
    uint32 Register_R12;
    uint32 Register_R14;
    uint32 Register_R15;
    uint32 Register_XPSR;
}StackFrame_Type;

#endif

