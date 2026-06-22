/* linker.ld — MCXN236 (verify addresses from RM chapter 2) */

MEMORY
{
    FLASH  (rx)  : ORIGIN = 0x00000000, LENGTH = 1024K
    RAM    (rwx) : ORIGIN = 0x20000000, LENGTH = 32K
    RES_STACK    (rwx) : ORIGIN = 0x20008000, LENGTH = 1024
}

SECTIONS
{
    /* ---- Vector table + code ---- */
    .text : ALIGN(4)
    {
        KEEP(*(.vectors))       /* vector table MUST be first */
        *(.text*)
        *(.rodata*)
        . = ALIGN(4);
        _etext = .;             /* marks end of Flash content */
    } > FLASH

    /* ---- Initialized data — lives in Flash, runs in RAM ---- */
    .data : ALIGN(4)
    {
        _sdata = .;
        *(.data*)
        . = ALIGN(4);
        _edata = .;
    } > RAM AT > FLASH         /* VMA = RAM, LMA = Flash */

    _sidata = LOADADDR(.data); /* Flash address of .data initial values */

    /* ---- Zero-initialized data ---- */
    .bss : ALIGN(4)
    {
        _sbss = .;
        KEEP(*(.bss*))
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
    } > RAM
    
    /* ---- Stack (grows downward from _estack) ---- */
    ._stack : ALIGN(8)
    {
        . = . + 0x800;          /* 2KB for MSP stack */
    } > RAM
}

/* Top of RAM = initial stack pointer */
_estack = ORIGIN(RES_STACK) + LENGTH(RES_STACK);
_estack_start = ORIGIN(RAM);
_estack_len = 1024;
