    .syntax unified
    .cpu    cortex-m33
    .thumb

    .global vector_table_NS
    .global Reset_Handler_NS
    .global SecureFault_Handler
    .global HardFault_Handler_NS
    .global Default_Handler_NS
    .global PendSV_Handler_NS
    .global SysTick_Handler_NS
    .global SVC_Handler_NS

/* =================================================================
 * SECURE VECTOR TABLE
 * ================================================================= */
    .section .vectors_ns, "a", %progbits
    .align 7
vector_table_NS:
    .word  _estack_ns               /*  0: Initial Secure MSP             */
    .word  Reset_Handler_NS         /*  1: Reset                          */
    .word  Default_Handler_NS       /*  2: NMI                            */
    .word  HardFault_Handler_NS     /*  3: HardFault                      */
    .word  Default_Handler_NS       /*  4: MemManage                      */
    .word  Default_Handler_NS       /*  5: BusFault                       */
    .word  Default_Handler_NS       /*  6: UsageFault                     */
    .word  SecureFault_Handler     /*  7: SecureFault                    */
    .word  0                       /*  8: Reserved                       */
    .word  0                       /*  9: Reserved                       */
    .word  0                       /* 10: Reserved                       */
    .word  SVC_Handler_NS           /* 11: SVCall                         */
    .word  Default_Handler_NS       /* 12: DebugMon                       */
    .word  0                       /* 13: Reserved                       */
    .word  PendSV_Handler_NS        /* 14: PendSV                         */
    .word  SysTick_Handler_NS       /* 15: SysTick                        */

/* =================================================================
 * RESET HANDLER
 * ================================================================= */
    .section .text.startup_NS
    .thumb_func
    .type  Reset_Handler_NS, %function
Reset_Handler_NS:
    cpsid   i
    
    b       .
    .size  Reset_Handler_NS, . - Reset_Handler_NS

/* =================================================================
 * SECUREFAULT HANDLER
 * ================================================================= */
    .section .text.startup_NS
    .thumb_func
    .type  SecureFault_Handler, %function
SecureFault_Handler:
    b       .
    .size  SecureFault_Handler, . - SecureFault_Handler

/* =================================================================
 * HARDFAULT HANDLER
 * ================================================================= */
    .section .text.startup_NS
    .thumb_func
    .type  HardFault_Handler_NS, %function
HardFault_Handler_NS:
    b       .
    .size  HardFault_Handler_NS, . - HardFault_Handler_NS

/* =================================================================
 * PendSV / SysTick / SVC / Default Handlers
 * ================================================================= */
    .section .text.startup_NS

    .thumb_func
    .weak  Default_Handler_NS
    .type  Default_Handler_NS, %function
Default_Handler_NS:
    b       .
    .size  Default_Handler_NS, . - Default_Handler_NS

    .thumb_func
    .weak  PendSV_Handler_NS
    .type  PendSV_Handler_NS, %function
PendSV_Handler_NS:
    bx      lr
    .size  PendSV_Handler_NS, . - PendSV_Handler_NS

    .thumb_func
    .weak  SysTick_Handler_NS
    .type  SysTick_Handler_NS, %function
SysTick_Handler_NS:
    bx      lr
    .size  SysTick_Handler_NS, . - SysTick_Handler_NS

    .thumb_func
    .weak  SVC_Handler_NS
    .type  SVC_Handler_NS, %function
SVC_Handler_NS:
    bx      lr
    .size  SVC_Handler_NS, . - SVC_Handler_NS
