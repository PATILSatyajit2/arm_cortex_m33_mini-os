MEMORY
{
    FLASH_ROOT (rx) :ORIGIN = 0x00000000, LENGTH = 1K
    FLASH_NS  (rx)  : ORIGIN = 0x00020000, LENGTH = 1K            /* Non-Secure Code */
    RAM_NS     (rwx) : ORIGIN = 0x30020000, LENGTH = 32K
}

/* Secure MSP initial value */
_estack_ns = ORIGIN(RAM_NS) + LENGTH(RAM_NS);

/* Address of NS vector table */
__ns_entry_addr = 0x00020000;

SECTIONS
{
    /* Secure vector table and secure code */
    .text : ALIGN(4)
    {
        KEEP(*(.vectors_ns))
        *(.text*)
        *(.rodata*)
        . = ALIGN(4);
        _etext_s = .;
    } > FLASH_NS
    
    .data : ALIGN(4)
    {
        _sdata_ns = .;
        *(.data*)
        . = ALIGN(4);
        _edata_ns = .;
    } > RAM_NS AT > FLASH_NS

    _sidata_s = LOADADDR(.data);

    .bss : ALIGN(4)
    {
        _sbss_ns = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss_ns = .;
    } > RAM_NS
}
