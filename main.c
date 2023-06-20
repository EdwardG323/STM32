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

struct gpio {
    volatile uint32_t MODER, OTYPER, OSPEEDER, PUPDR, IDR, ODR, BSRR, LCKR, AFR[2];
};

// #define GPIOA ((struct gpio *) 0x42020000)
#define GPIO(bank) ((struct gpio *) (0x42020000 + 0x400 * (bank)))

#define PIN(bank, num) ((((bank) - 'A') << 8) | (num))
#define PINNO(pin) (pin & 255)
#define PINBANK(pin) (pin >> 8) 

enum {
    GPIO_MODE_INPUT,
    GPIO_MODE_OUTPUT,
    GPIO_MODE_AF,
    GPIO_MODE_ANALOG
};


// * (volatile uint32_t *) (0x42020000 + 0) = 0;	// Sets pins A0 - A15 to inputs

// * (volatile uint32_t *) (0x42020000 + 0) &= 3 << 6;	// Clear bit ranges 6-7
// * (volatile uint32_t *) (0x42020000 + 0) |= 1 << 6;	// set bit range 6-7 to 1

static inline void gpio_set_mode(uint16_t pin, uint8_t mode){
    struct gpio *gpio = GPIO(PINBANK(pin));     // GPIO bank
    uint8_t n = PINNO(pin);                     // Pin number
    gpio->MODER &= ~(3U << (n * 2));          // Clear existing bits
    gpio->MODER |= (mode & 3) << (n * 2);     // Set new bits    
}

// Startup code
__attribute__((naked, noreturn)) void _reset(void){
    for(;;) (void) 0;       // Infinite loop
}

// 16 standard and 91 STM32-specific handlers
__attribute__((section(".vectors"))) void (*tab[16+91])(void) = {
    _estack, _reset
}