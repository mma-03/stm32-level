#include <stdint.h>

extern uint32_t _sidata; // start of intialized data in FLASH
extern uint32_t _sdata;  // start of .data in RAM
extern uint32_t _edata;  // ends of .data in RAM
extern uint32_t _sbss;   // start of .bss in RAM
extern uint32_t _ebss;   // end of .bss in RAM
extern uint32_t _estack; // top of stack (RAM)

int main(void);
void Reset_Handler(void);

uint32_t *vector_table[] __attribute__((section(".isr_vector"))) = {
    (uint32_t *)&_estack,
    (uint32_t *)Reset_Handler
};

void Reset_Handler(void) {
    // 1. Copy data from FLASH to RAM
    // two walking pointers, source and destination
    // loop until destination reaches the end

    uint32_t *src = &_sidata;
    uint32_t *dst = &_sdata;

    while(dst < &_edata) {
        *dst = *src;
        src++;
        dst++;
    }

    // 2. Zero .bss
    // one walking pointer
    // write zeros until it reachs the end
    
    uint32_t *bss = &_sbss;

    while(bss < &_ebss) {
        *bss = 0;
        bss++;
    }

    main();
    while(1);
}