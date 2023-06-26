/****************************************************************************
 Module
     main.c
 Description
     starter code for STM32 firware
 Notes

 History
 When           Who     What/Why
 -------------- ---     --------
 06/18/23 5:30 Edward   started coding
*****************************************************************************/
#include <inttypes.h>
#include <stdbool.h>

struct gpio {
    volatile uint32_t MODER, OTYPER, OSPEEDER, PUPDR, IDR, ODR, BSRR, LCKR, AFR[2];
};


#define GPIOA ((struct gpio *) 0x42020000)
#define GPIO(bank) ((struct gpio *) (0x42020000 + 0x400 * (bank)))

#define BIT(x) (1UL << (x))
#define PIN(bank, num) ((((bank) - 'A') << 8) | (num))
#define PINNO(pin) (pin & 255)
#define PINBANK(pin) (pin >> 8) 

enum {
    GPIO_MODE_INPUT,
    GPIO_MODE_OUTPUT,
    GPIO_MODE_AF,
    GPIO_MODE_ANALOG
};


* (volatile uint32_t *) (0x42020000 + 0) = 0;	// Sets pins A0 - A15 to inputs

* (volatile uint32_t *) (0x42020000 + 0) &= 3 << 6;	// Clear bit ranges 6-7
* (volatile uint32_t *) (0x42020000 + 0) |= 1 << 6;	// set bit range 6-7 to 1

static inline void gpio_set_mode(uint16_t pin, uint8_t mode){
    struct gpio *gpio = GPIO(PINBANK(pin));     // GPIO bank
    uint8_t n = PINNO(pin);                     // Pin number
    gpio->MODER &= ~(3U << (n * 2));          // Clear existing bits
    gpio->MODER |= (mode & 3) << (n * 2);     // Set new bits    
}

/*
 The Reset and Clock Control (RCC) unit is used to enable (clock) 
 peripherial buses. On STM32 MCUs, the desired peripheral must be
 enabled in order for read/ write access.
*/
struct rcc {
    volatile uint32_t CR, Reserved0, ICSCR1, ICSCR2, ICSCR3, CRRCR, Reserved1,
    CFGR1, CFGR2, CFGR3, PLL1CFGR, PLL2CFGR, PLL3CFGR, PLL1DIVR, PLL1FRACR,
    PLL2DIVR, PLL2FRACR, PLL3DIVR, PLL3FRACR, Reserved2, CIER, CIFR, CICR,
    Reserved3, AHB1RSTR, AHB2RSTR1, AHB2RSTR2, AHB3RSTR3, Reserved4, 
    APB1RSTR1, APB1RSTR2, APB2RSTR, APB3RSTR, Reserved5, AHB1ENR, AHB2ENR1,
    AHB2ENR2, AHB3ENR, APB1ENR1, APB1ENR2, APB2ENR, APB3ENR, Reserved6, 
    AHB1SMENR, AHB2SMENR1, AHB2SMENR2, AHB3SMENR, Reserved7, APB1SMENR1, APB1SMENR2,
    APB2SMENR, APB3SMENR, Reserved8, SRDAMR, CCIPR1, CCIPR2, CCIPR3, BDCR, CSR,
    Reserved9[2], SECCFGR, PRIVCFGR;
};

#define RCC ((struct rcc* ) 0x46020c00) 

int main(void){
    return 0;   // Do nothing for now
}

// Startup code
__attribute__((naked, noreturn)) void _reset(void){
    // memset .bss to zero, copy .data section to RAM
    extern long _sbss, _ebss, _sdata, _edata, _sidata;

    // Zero .bss section
    for(long *src = &_sbss; src < &_ebss; src++)
        *src = 0;

    // Copy .data from flash to RAM
    for(long *src = &_sdata, *dst = &_sidata; src < &_edata;)
        *src++ = *dst++;

    main();                 // call main
    for(;;) (void) 0;       // Infinite loop
}

extern void _estack(void);

// 16 standard and 141 STM32-specific handlers
__attribute__((section(".vectors"))) void (*tab[16+141])(void) = {
    _estack, _reset
};