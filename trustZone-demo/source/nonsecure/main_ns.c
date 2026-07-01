#include <stdint.h>
// #include "nsc_api.h"

void ns_event_callback(uint32_t event_data)
{
}

static void delay_cycles(uint32_t n)
{
    for (volatile uint32_t i = 0; i < n; i++);
}

void main_nonsecure(void)
{
    while (1) {
    }
}
