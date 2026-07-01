#include <stdint.h>
#include "sau_init.h"

/* SAU Registers */
#define SAU_BASE  0xE000EDD0UL
typedef struct {
    volatile uint32_t CTRL;   /* +0x00 Control                     */
    volatile uint32_t TYPE;   /* +0x04 Type (read-only, # regions) */
    volatile uint32_t RNR;    /* +0x08 Region Number               */
    volatile uint32_t RBAR;   /* +0x0C Region Base Address         */
    volatile uint32_t RLAR;   /* +0x10 Region Limit Address+attrs  */
    volatile uint32_t SFSR;   /* +0x14 SecureFault Status          */
    volatile uint32_t SFAR;   /* +0x18 SecureFault Address         */
} SAU_Type;

#define SAU_REGION_0_BASE   0x00020000u
#define SAU_REGION_0_END    0x00020400u
#define SAU_REGION_1_BASE   0x30020000u
#define SAU_REGION_1_END    0x00040000u

#define SAU                 ((SAU_Type *)SAU_BASE)

#define SAU_ADDR_ALIGNMENT    (0x7FFFFFFUL << 5)
#define SAU_RLAR_ENABLE_Msk   1u
#define SAU_RLAR_NSC_Msk      2u


void sau_init(void)
{
  SAU->CTRL = 0;
  
  SAU->RNR = 0;
  SAU->RBAR = SAU_REGION_0_BASE & SAU_ADDR_ALIGNMENT;
  SAU->RLAR = (SAU_REGION_0_END & SAU_ADDR_ALIGNMENT)
        | ((0U << 1) & SAU_RLAR_NSC_Msk)
        | ((1U << 0) & SAU_RLAR_ENABLE_Msk);

  SAU->RNR = 1;
  SAU->RBAR = SAU_REGION_1_BASE & SAU_ADDR_ALIGNMENT;
  SAU->RLAR = (SAU_REGION_1_END & SAU_ADDR_ALIGNMENT)
        | ((0U << 1) & SAU_RLAR_NSC_Msk)
        | ((1U << 0) & SAU_RLAR_ENABLE_Msk);

  SAU->CTRL = 1;
}
