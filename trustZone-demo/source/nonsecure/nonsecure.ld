/* =================================================================
 * nonsecure.ld  —  MCXN236 Non-secure World Linker Script
 *
 * NS code is linked to NS ALIAS addresses.
 * The IDAU maps:
 *   Physical 0x00080000 <-> NS alias 0x10080000
 *   Physical 0x20020000 <-> NS alias 0x30020000
 *
 * When Secure world flashes NS binary at physical 0x00080000,
 * the Cortex-M33 in NS state sees it at 0x10080000.
 * ================================================================= */

MEMORY
{
    FLASH_NS (rx)  : ORIGIN = 0x10080000, LENGTH = 512K
    RAM_NS   (rwx) : ORIGIN = 0x30020000, LENGTH = 128K
}

_estack_ns = ORIGIN(RAM_NS) + LENGTH(RAM_NS);

SECTIONS
{
    /* NS vector table at 0x10080000.
     * Secure world reads this to get NS MSP (word 0) and
     * NS entry point (word 1) before calling BLXNS. */
    .text : ALIGN(4)
    {
        KEEP(*(.vectors_ns))
        *(.text.NS_Entry)
        *(.text*)
        *(.rodata*)
        . = ALIGN(4);
    } > FLASH_NS

    .data : ALIGN(4)
    {
        _sdata_ns = .;
        *(.data*)
        . = ALIGN(4);
        _edata_ns = .;
    } > RAM_NS AT > FLASH_NS

    _sidata_ns = LOADADDR(.data);

    .bss : ALIGN(4)
    {
        _sbss_ns = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss_ns = .;
    } > RAM_NS
}
