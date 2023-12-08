/****************************************************************************
 Module
     main.c
 Description
     starter code for STM32 firware
 Notes
      Eventually move the register structs to their own .C file to make this
      one cleaner
      
 History
 When           Who     What/Why
 -------------- ---     --------
 06/18/23 5:30 Edward   started coding
*****************************************************************************/
#include <inttypes.h>
#include <stdbool.h>

struct gpio {
    volatile uint32_t MODER, OTYPER, OSPEEDER, PUPDR, IDR, ODR, BSRR, LCKR, AFRL,
    AFRH, BRR/*, HSLVR, SECCFGR*/;
};

#define RCC_AHBENR_GPIOAEN_Pos (17U)                         
#define RCC_AHBENR_GPIOAEN_Msk (0x1UL << RCC_AHBENR_GPIOAEN_Pos) /*!< 0x00020000 */

//#define GPIOA ((struct gpio *) 0x42020000)
// For the U575
//#define GPIO(bank) ((struct gpio *) (0x42020000 + 0x400 * (bank)))

//For the F303
#define GPIO(bank) ((struct gpio *) (0x48000000 + 0x400 * (bank)))

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


// // * (volatile uint32_t *) (0x42020000 + 0) = 0;	// Sets pins A0 - A15 to inputs

// // * (volatile uint32_t *) (0x42020000 + 0) &= 3 << 6;	// Clear bit ranges 6-7
// // * (volatile uint32_t *) (0x42020000 + 0) |= 1 << 6;	// set bit range 6-7 to 1

static inline void gpio_set_mode(uint16_t pin, uint8_t mode){
    struct gpio *gpio = GPIO(PINBANK(pin));     // GPIO bank
    uint8_t n = PINNO(pin);                     // Pin number
    gpio->MODER &= ~(3U << (n * 2));          // Clear existing bits
    gpio->MODER |= (mode & 3) << (n * 2);     // Set new bits    
}

static inline void gpio_write(uint16_t pin, bool val){
    struct gpio *gpio = GPIO(PINBANK(pin));
    gpio->BSRR = (1u << PINNO(pin)) << (val ? 0 : 16);
}

static inline void spin(volatile uint32_t count){
    while (count--) (void) 0;
}

/*
 The Reset and Clock Control (RCC) unit is used to enable (clock) 
 peripherial buses. On STM32 MCUs, the desired peripheral must be
 enabled in order for read/ write access.
*/
// struct rcc_U575 {
//     volatile uint32_t CR, Reserved0, ICSCR1, ICSCR2, ICSCR3, CRRCR, Reserved1,
//     CFGR1, CFGR2, CFGR3, PLL1CFGR, PLL2CFGR, PLL3CFGR, PLL1DIVR, PLL1FRACR,
//     PLL2DIVR, PLL2FRACR, PLL3DIVR, PLL3FRACR, Reserved2, CIER, CIFR, CICR,
//     Reserved3, AHB1RSTR, AHB2RSTR1, AHB2RSTR2, AHB3RSTR3, Reserved4, 
//     APB1RSTR1, APB1RSTR2, APB2RSTR, APB3RSTR, Reserved5, AHB1ENR, AHB2ENR1,
//     AHB2ENR2, AHB3ENR, APB1ENR1, APB1ENR2, APB2ENR, APB3ENR, Reserved6, 
//     AHB1SMENR, AHB2SMENR1, AHB2SMENR2, AHB3SMENR, Reserved7, APB1SMENR1, APB1SMENR2,
//     APB2SMENR, APB3SMENR, Reserved8, SRDAMR, CCIPR1, CCIPR2, CCIPR3, BDCR, CSR,
//     Reserved9[2], SECCFGR, PRIVCFGR;
// };
struct rcc {
    volatile uint32_t CR, CFGR, CIR, APB2RSTR, APB1RSTR, AHBENR, APB2ENR, APB1ENR,
    BDCR, CSR, AHBRSTR, CFGR2, CFGR3;
};

// #define RCC_U575 ((struct rcc_U575* ) 0x46020c00)

// Found on page 55 of user manual
#define RCC ((struct rcc* ) 0x40021000) 

int main(void){
    uint16_t led = PIN('B', 7);
    // RCC->AHB2ENR1 |= BIT(PINBANK(led));
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN_Msk;
    gpio_set_mode(led, GPIO_MODE_OUTPUT);
    gpio_write(led, true);

    /*for(;;){
        gpio_write(led, true);
        spin(999999);
        gpio_write(led, false);
        spin(999999);
    }*/
    return 0;   // Do nothing for now
}

