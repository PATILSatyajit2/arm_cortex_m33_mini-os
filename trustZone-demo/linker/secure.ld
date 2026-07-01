MEMORY
{
    FLASH_S   (rx)  : ORIGIN = 0x00000000, LENGTH = 127K          /* Secure Code */
    FLASH_NSC (rx)  : ORIGIN = 0x0001FC00, LENGTH = 1K            /* Non-Secure Callable */
    FLASH_NS  (rx)  : ORIGIN = 0x00020000, LENGTH = 1K            /* Non-Secure Code */
    RAM_S     (rwx) : ORIGIN = 0x30000000, LENGTH = 32K           /* Secure ram for stack and data - RAM-A aliased */
}

/* Secure MSP initial value */
_estack_s = ORIGIN(RAM_S) + LENGTH(RAM_S);

/* Address of NS vector table */
__ns_entry_addr = 0x00020000;

SECTIONS
{
    /* Secure vector table and secure code */
    .text : ALIGN(4)
    {
        KEEP(*(.vectors_s))
        *(.text.startup_s)
        *(.text*)
        *(.rodata*)
        . = ALIGN(4);
        _etext_s = .;
    } > FLASH_S
    
    /* vaneer table */
    .gnu.sgstubs : ALIGN(32)
    {
        _start_sg = .;
        *(.gnu.sgstubs*)
        . = ALIGN(32);
        _end_sg = .;
    } > FLASH_NSC

    /* Secure data */
    .data : ALIGN(4)
    {
        _sdata_s = .;
        *(.data*)
        . = ALIGN(4);
        _edata_s = .;
    } > RAM_S AT > FLASH_S

    _sidata_s = LOADADDR(.data);

    /* Secure bss */
    .bss : ALIGN(4)
    {
        _sbss_s = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss_s = .;
    } > RAM_S
}
