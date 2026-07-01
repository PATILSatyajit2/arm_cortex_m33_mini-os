#include <stdint.h>
#include "sau_init.h"
#include "nsc_api.h"

#define     NS_ENTRY    0x00020000u
#define     __IM     volatile const      /*! Defines 'read only' structure member permissions */
#define     __OM     volatile            /*! Defines 'write only' structure member permissions */
#define     __IOM    volatile            /*! Defines 'read / write' structure member permissions */

typedef struct
{
  __IM  uint32_t CPUID;                  /*!< Offset: 0x000 (R/ )  CPUID Base Register */
  __IOM uint32_t ICSR;                   /*!< Offset: 0x004 (R/W)  Interrupt Control and State Register */
  __IOM uint32_t VTOR;                   /*!< Offset: 0x008 (R/W)  Vector Table Offset Register */
  __IOM uint32_t AIRCR;                  /*!< Offset: 0x00C (R/W)  Application Interrupt and Reset Control Register */
  __IOM uint32_t SCR;                    /*!< Offset: 0x010 (R/W)  System Control Register */
  __IOM uint32_t CCR;                    /*!< Offset: 0x014 (R/W)  Configuration Control Register */
  __IOM uint8_t  SHPR[12U];              /*!< Offset: 0x018 (R/W)  System Handlers Priority Registers (4-7, 8-11, 12-15) */
  __IOM uint32_t SHCSR;                  /*!< Offset: 0x024 (R/W)  System Handler Control and State Register */
  __IOM uint32_t CFSR;                   /*!< Offset: 0x028 (R/W)  Configurable Fault Status Register */
  __IOM uint32_t HFSR;                   /*!< Offset: 0x02C (R/W)  HardFault Status Register */
  __IOM uint32_t DFSR;                   /*!< Offset: 0x030 (R/W)  Debug Fault Status Register */
  __IOM uint32_t MMFAR;                  /*!< Offset: 0x034 (R/W)  MemManage Fault Address Register */
  __IOM uint32_t BFAR;                   /*!< Offset: 0x038 (R/W)  BusFault Address Register */
  __IOM uint32_t AFSR;                   /*!< Offset: 0x03C (R/W)  Auxiliary Fault Status Register */
  __IM  uint32_t ID_PFR[2U];             /*!< Offset: 0x040 (R/ )  Processor Feature Register */
  __IM  uint32_t ID_DFR;                 /*!< Offset: 0x048 (R/ )  Debug Feature Register */
  __IM  uint32_t ID_AFR;                 /*!< Offset: 0x04C (R/ )  Auxiliary Feature Register */
  __IM  uint32_t ID_MMFR[4U];            /*!< Offset: 0x050 (R/ )  Memory Model Feature Register */
  __IM  uint32_t ID_ISAR[6U];            /*!< Offset: 0x060 (R/ )  Instruction Set Attributes Register */
  __IM  uint32_t CLIDR;                  /*!< Offset: 0x078 (R/ )  Cache Level ID register */
  __IM  uint32_t CTR;                    /*!< Offset: 0x07C (R/ )  Cache Type register */
  __IM  uint32_t CCSIDR;                 /*!< Offset: 0x080 (R/ )  Cache Size ID Register */
  __IOM uint32_t CSSELR;                 /*!< Offset: 0x084 (R/W)  Cache Size Selection Register */
  __IOM uint32_t CPACR;                  /*!< Offset: 0x088 (R/W)  Coprocessor Access Control Register */
  __IOM uint32_t NSACR;                  /*!< Offset: 0x08C (R/W)  Non-Secure Access Control Register */
        uint32_t RESERVED7[21U];
  __IOM uint32_t SFSR;                   /*!< Offset: 0x0E4 (R/W)  Secure Fault Status Register */
  __IOM uint32_t SFAR;                   /*!< Offset: 0x0E8 (R/W)  Secure Fault Address Register */
        uint32_t RESERVED3[69U];
  __OM  uint32_t STIR;                   /*!< Offset: 0x200 ( /W)  Software Triggered Interrupt Register */
        uint32_t RESERVED4[15U];
  __IM  uint32_t MVFR0;                  /*!< Offset: 0x240 (R/ )  Media and VFP Feature Register 0 */
  __IM  uint32_t MVFR1;                  /*!< Offset: 0x244 (R/ )  Media and VFP Feature Register 1 */
  __IM  uint32_t MVFR2;                  /*!< Offset: 0x248 (R/ )  Media and VFP Feature Register 2 */
        uint32_t RESERVED5[1U];
  __OM  uint32_t ICIALLU;                /*!< Offset: 0x250 ( /W)  I-Cache Invalidate All to PoU */
        uint32_t RESERVED6[1U];
  __OM  uint32_t ICIMVAU;                /*!< Offset: 0x258 ( /W)  I-Cache Invalidate by MVA to PoU */
  __OM  uint32_t DCIMVAC;                /*!< Offset: 0x25C ( /W)  D-Cache Invalidate by MVA to PoC */
  __OM  uint32_t DCISW;                  /*!< Offset: 0x260 ( /W)  D-Cache Invalidate by Set-way */
  __OM  uint32_t DCCMVAU;                /*!< Offset: 0x264 ( /W)  D-Cache Clean by MVA to PoU */
  __OM  uint32_t DCCMVAC;                /*!< Offset: 0x268 ( /W)  D-Cache Clean by MVA to PoC */
  __OM  uint32_t DCCSW;                  /*!< Offset: 0x26C ( /W)  D-Cache Clean by Set-way */
  __OM  uint32_t DCCIMVAC;               /*!< Offset: 0x270 ( /W)  D-Cache Clean and Invalidate by MVA to PoC */
  __OM  uint32_t DCCISW;                 /*!< Offset: 0x274 ( /W)  D-Cache Clean and Invalidate by Set-way */
  __OM  uint32_t BPIALL;                 /*!< Offset: 0x278 ( /W)  Branch Predictor Invalidate All */
} SCB_Type;

#define SCS_BASE_NS         (0xE002E000UL)
#define SCB_BASE_NS         (SCS_BASE_NS +  0x0D00UL)
#define SCB_NS              ((SCB_Type *)SCB_BASE_NS)

void secure_fault_c_handler(uint32_t *frame, uint32_t lr)
{
    while (1);
}

void hard_fault_c_handler(uint32_t *frame, uint32_t lr)
{
    __asm volatile ("bkpt #2");
    while (1);
}

void nonSecureFunction(void)
{
    __asm volatile ("mov r0, #0");
    __asm volatile ("mov r1, #1");
    __asm volatile ("mov r2, #2");
}

__attribute__((cmse_nonsecure_entry))
void nonSecureFunction_Wrapper(void)
{
    __asm volatile ("mov r0, #0");
    __asm volatile ("mov r1, #1");
    __asm volatile ("mov r2, #2");
}

void setMSP_ns(uint32_t stackTop)
{
  __asm volatile ("MSR msp_ns, %0" : : "r" (stackTop) : );
}

/*============================================================================*/
/*============================================================================*/

#define CPACR    (*(volatile unsigned int*)0xE000ED88u)
#define TZM_IS_NONSECURE_CALLED __attribute__((cmse_nonsecure_call))
typedef void (*funcptr_ns)(void) TZM_IS_NONSECURE_CALLED;

void jumpToNormal(unsigned int ns_entry_addr)
{
  funcptr_ns ResetHandler_ns;
  
  SCB_NS->VTOR = ns_entry_addr;
  setMSP_ns(*(unsigned int*)ns_entry_addr);
  
  ResetHandler_ns = (funcptr_ns)(*((uint32_t *)((ns_entry_addr) + 4U)));
  
  CPACR = 0xFFFFFFFF;
  ResetHandler_ns();
}

/*============================================================================*/
/*============================================================================*/

void main_secure(void)
{
  //nonSecureFunction_Wrapper();
  sau_init();
  jumpToNormal(NS_ENTRY);
  
  __asm volatile("mov r0,#0x20000000");
  __asm volatile("msr msp_ns, r0");
  
  __asm volatile("ldr r0,=nonSecureFunction");
  __asm volatile("bxns r0");
  while (1);
}
