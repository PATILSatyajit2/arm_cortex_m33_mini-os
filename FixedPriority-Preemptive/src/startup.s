/* startup.s — Vector table + Reset handler for MCXN236 */
    .syntax unified
    .cpu cortex-m33
    .thumb
    
    .global Reset_Handler
    .global PendSV_Handler
    .global SysTick_Handler
    .global Svc_Handler
    .global Default_Handler
    .global Os_Systick_Handler
  
/* ---- Vector table ---- */
    .section .vectors,"a",%progbits
    .align 2
    .global vector_table
vector_table:
    .word   _estack                 /* 0: initial MSP value              */
    .word   Reset_Handler           /* 1: Reset                          */
    .word   Default_Handler         /* 2: NMI                            */
    .word   Default_Handler         /* 3: HardFault                      */
    .word   Default_Handler         /* 4: MemManage                      */
    .word   Default_Handler         /* 5: BusFault                       */
    .word   Default_Handler         /* 6: UsageFault                     */
    .word   0                       /* 7-10: reserved                    */
    .word   0
    .word   0
    .word   0
    .word   Svc_Handler             /* 11: SVCall                        */
    .word   Default_Handler         /* 12: DebugMon                      */
    .word   0                       /* 13: reserved                      */
    .word   PendSV_Handler          /* 14: PendSV  ← context switch      */
    .word   Os_Systick_Handler      /* 15: SysTick ← OS tick             */
    /* Add peripheral IRQs here as needed */
    
.type Reset_Handler, %function
Reset_Handler:
    mov r0, #1
    mov r1, #2
    mov r2, #3
    mov r3, #4
    mov r4, #5
    
Default_Ram_Init:
    ldr r0, =_estack_start
    ldr r1, =_estack_len
    ldr r2, =0x5a505a50
ram_init:
    str r2, [r0]
    sub r1, #1
    add r0, #4
    CMP r1, #0
    BNE ram_init
bss_clear:
    ldr r0,=_sbss
    ldr r1,=_ebss
    mov r2, #0
    B   bss_zero_check
bss_clear_loop:
    str r2, [r0], #4
bss_zero_check:
    cmp r0, r1
    blt bss_clear_loop
    bl main
    
.section .text
.type Svc_Handler, %function
Svc_Handler:
    BX  LR

PendSV_Handler:
    nop
    
.align 4
.section .data
.global systick_counter
systick_counter:
  .word 0

.section .text
.type SysTick_Handler, %function
SysTick_Handler:
    push {r4-r11}
    # ldr r0,=Os_Systick_Handler
    # orr r0, #1
    bl Os_Systick_Handler
    pop  {r4-r11}
    BX LR

Default_Handler:
    nop

.end
