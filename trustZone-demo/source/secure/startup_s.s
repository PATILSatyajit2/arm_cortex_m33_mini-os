    .syntax unified
    .cpu    cortex-m33
    .thumb

    .global vector_table_s
    .global Reset_Handler_S
    .global SecureFault_Handler
    .global HardFault_Handler_S
    .global Default_Handler_S
    .global PendSV_Handler_S
    .global SysTick_Handler_S
    .global SVC_Handler_S

/* =================================================================
 * SECURE VECTOR TABLE
 * ================================================================= */
    .section .vectors_s, "a", %progbits
    .align 7
vector_table_s:
    .word  _estack_s               /*  0: Initial Secure MSP             */
    .word  Reset_Handler_S         /*  1: Reset                          */
    .word  Default_Handler_S       /*  2: NMI                            */
    .word  HardFault_Handler_S     /*  3: HardFault                      */
    .word  Default_Handler_S       /*  4: MemManage                      */
    .word  Default_Handler_S       /*  5: BusFault                       */
    .word  Default_Handler_S       /*  6: UsageFault                     */
    .word  SecureFault_Handler     /*  7: SecureFault                    */
    .word  0                       /*  8: Reserved                       */
    .word  0                       /*  9: Reserved                       */
    .word  0                       /* 10: Reserved                       */
    .word  SVC_Handler_S           /* 11: SVCall                         */
    .word  Default_Handler_S       /* 12: DebugMon                       */
    .word  0                       /* 13: Reserved                       */
    .word  PendSV_Handler_S        /* 14: PendSV                         */
    .word  SysTick_Handler_S       /* 15: SysTick                        */

/* =================================================================
 * RESET HANDLER
 * ================================================================= */
    .section .text.startup_s
    .thumb_func
    .type  Reset_Handler_S, %function
Reset_Handler_S:
    cpsid   i
    
    /* MSP will already be loaded with _estack_s */
    ldr     r0, =_estack_s
    msr     msp, r0

    /* Secure data copy */
    ldr     r0, =_sidata_s
    ldr     r1, =_sdata_s
    ldr     r2, =_edata_s
    b       .Ls_data_check
.Ls_data_loop:
    ldr     r3, [r0], #4
    str     r3, [r1], #4
.Ls_data_check:
    cmp     r1, r2
    blt     .Ls_data_loop

    /* Secure bss */
    ldr     r0, =_sbss_s
    ldr     r1, =_ebss_s
    movs    r2, #0
    b       .Ls_bss_check
.Ls_bss_loop:
    str     r2, [r0], #4
.Ls_bss_check:
    cmp     r0, r1
    blt     .Ls_bss_loop

    cpsie   i

    bl      main_secure
    
    b       .                                 /* Execution should never reach here */
    .size  Reset_Handler_S, . - Reset_Handler_S

/* =================================================================
 * SECUREFAULT HANDLER
 * ================================================================= */
    .section .text.startup_s
    .thumb_func
    .type  SecureFault_Handler, %function
SecureFault_Handler:
    /* Determine active stack */
    tst     lr, #4
    ite     eq
    mrseq   r0, msp
    mrsne   r0, psp
    mov     r1, lr
    b       secure_fault_c_handler
    .size  SecureFault_Handler, . - SecureFault_Handler

/* =================================================================
 * HARDFAULT HANDLER
 * ================================================================= */
    .section .text.startup_s
    .thumb_func
    .type  HardFault_Handler_S, %function
HardFault_Handler_S:
    tst     lr, #4
    ite     eq
    mrseq   r0, msp
    mrsne   r0, psp
    mov     r1, lr
    b       hard_fault_c_handler
    .size  HardFault_Handler_S, . - HardFault_Handler_S

/* =================================================================
 * PendSV / SysTick / SVC / Default Handlers
 * ================================================================= */
    .section .text.startup_s

    .thumb_func
    .weak  Default_Handler_S
    .type  Default_Handler_S, %function
Default_Handler_S:
    b       .
    .size  Default_Handler_S, . - Default_Handler_S

    .thumb_func
    .weak  PendSV_Handler_S
    .type  PendSV_Handler_S, %function
PendSV_Handler_S:
    bx      lr
    .size  PendSV_Handler_S, . - PendSV_Handler_S

    .thumb_func
    .weak  SysTick_Handler_S
    .type  SysTick_Handler_S, %function
SysTick_Handler_S:
    bx      lr
    .size  SysTick_Handler_S, . - SysTick_Handler_S

    .thumb_func
    .weak  SVC_Handler_S
    .type  SVC_Handler_S, %function
SVC_Handler_S:
    bx      lr
    .size  SVC_Handler_S, . - SVC_Handler_S
