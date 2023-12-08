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

#define SRAM_START (0x20000000u)
#define SRAM_SIZE (32u * 1024u)
#define SRAM_END (SRAM_START + SRAM_SIZE)
#define STACK_PTR_INIT_ADD (SRAM_END)

#define ISR_VECTOR_SIZE 74+16+16

void reset_handler(void);
void default_handler(void);
void nmi_handler(void)__attribute__((weak, alias("default_handler")));
void hard_fault_handler(void)__attribute__((weak, alias("default_handler")));
void mem_manage_handler(void)__attribute__((weak, alias("default_handler")));
void bus_fault_handler(void)__attribute__((weak, alias("default_handler")));
void usage_handler(void)__attribute__((weak, alias("default_handler")));
void svcall_handler(void)__attribute__((weak,alias("default_handler")));
void pendsv_handler(void)__attribute__((weak, alias("default_handler")));
void systick_handler(void)__attribute__((weak, alias("default_handler")));
void wwdg_handler(void)__attribute__((weak, alias("default_handler")));
void pvd_handler(void)__attribute__((weak, alias("default_handler")));
void tamper_stamp_handler(void)__attribute__((weak, alias("default_handler")))
void rtc_wkup_handler(void)__attribute__((weak, alias("default_handler")));
void flash_handler(void)__attribute__((weak, alias("default_handler")));
void rcc_handler(void)__attribute__((weak, alias("default_handler")));
void exti0_handler(void)__attribute__((weak, alias("default_handler")));
void exti1_handler(void)__attribute__((weak, alias("default_handler")));
void exti2_ts_handler(void)__attribute__((weak, alias("default_handler")));
void exti3_handler(void)__attribute__((weak, alias("default_handler")));
void exti4_handler(void)__attribute__((weak, alias("default_handler")));
void dma1_channel1_handler(void)__attribute__((weak, alias("default_handler")));
void dma1_channel2_handler(void)__attribute__((weak, alias("default_handler")));
void dma1_channel3_handler(void)__attribute__((weak, alias("default_handler")));
void dma1_channel4_handler(void)__attribute__((weak, alias("default_handler")));
void dma1_channel5_handler(void)__attribute__((weak, alias("default_handler")));
void dma1_channel6_handler(void)__attribute__((weak, alias("default_handler")));
void dma1_channel7_handler(void)__attribute__((weak, alias("default_handler")));
void adc1_2_handler(void)__attribute__((weak, alias("default_handler")));
void usb_hp_can_tx_handler(void)__attribute__((weak, alias("default_handler")));
void usb_lp_can_rx0_handler(void)__attribute__((weak, alias("default_handler")));
void can_rx1_handler(void)__attribute__((weak, alias("default_handler")));
void can_sce_handler(void)__attribute__((weak, alias("default_handler")));
void exti9_5_handler(void)__attribute__((weak, alias("default_handler")));
void tim1_brk_tim15_handler(void)__attribute__((weak, alias("default_handler")));
void tim1_up_tim16_handler(void)__attribute__((weak, alias("default_handler")));
void tim1_trg_com_tim17_handler(void)__attribute__((weak, alias("default_handler")));
void tim1_cc_handler(void)__attribute__((weak, alias("default_handler")));
void tim2_handler(void)__attribute__((weak, alias("default_handler")));
void tim3_handler(void)__attribute__((weak, alias("default_handler")));
void tim4_handler(void)__attribute__((weak, alias("default_handler")));
void i2c1_ev_handler(void)__attribute__((weak, alias("default_handler")));
void i2c1_er_handler(void)__attribute__((weak, alias("default_handler")));
void i2c2_ev_handler(void)__attribute__((weak, alias("default_handler")));
void i2c2_er_handler(void)__attribute__((weak, alias("default_handler")));
void spi1_handler(void)__attribute__((weak, alias("default_handler")));
void spi2_handler(void)__attribute__((weak, alias("default_handler")));
void usart1_handler(void)__attribute__((weak, alias("default_handler")));
void usart2_handler(void)__attribute__((weak, alias("default_handler")));
void usart3_handler(void)__attribute__((weak, alias("default_handler")));
void exti15_10_handler(void)__attribute__((weak, alias("default_handler")));
void rtc_alarm_handler(void)__attribute__((weak, alias("default_handler")));
void usb_wakeup_handler(void)__attribute__((weak, alias("default_handler")));
void tim8_brk_handler(void)__attribute__((weak, alias("default_handler")));
void tim8_up_handler(void)__attribute__((weak, alias("default_handler")));
void tim8_trg_com_handler(void)__attribute__((weak, alias("default_handler")));
void tim8_cc_handler(void)__attribute__((weak, alias("default_handler")));
void adc3_handler(void)__attribute__((weak, alias("default_handler")));
void fmc_handler(void)__attribute__((weak, alias("default_handler")));
void spi3_handler(void)__attribute__((weak, alias("default_handler")));
void uart4_handler(void)__attribute__((weak, alias("default_handler")));
void uart5_handler(void)__attribute__((weak, alias("default_handler")));
void tim6_dac_handler(void)__attribute__((weak, alias("default_handler")));
void tim7_handler(void)__attribute__((weak, alias("default_handler")));
void dma2_channel1_handler(void)__attribute__((weak, alias("default_handler")));
void dma2_channel2_handler(void)__attribute__((weak, alias("default_handler")));
void dma2_channel3_handler(void)__attribute__((weak, alias("default_handler")));
void dma2_channel4_handler(void)__attribute__((weak, alias("default_handler")));
void dma2_channel5_handler(void)__attribute__((weak, alias("default_handler")));
void adc4_handler(void)__attribute__((weak, alias("default_handler")));
void comp123_handler(void)__attribute__((weak, alias("default_handler")));
void comp456_handler(void)__attribute__((weak, alias("default_handler")));
void comp7_handler(void)__attribute__((weak, alias("default_handler")));
void i2c3_ev_handler(void)__attribute__((weak, alias("default_handler")));
void i2c3_er_handler(void)__attribute__((weak, alias("default_handler")));
void usb_hp_handler(void)__attribute__((weak, alias("default_handler")));
void usb_lp_handler(void)__attribute__((weak, alias("default_handler")));
void usb_wakeup_rmp_handler(void)__attribute__((weak, alias("default_handler")));
void tim20_brk_handler(void)__attribute__((weak, alias("default_handler")));
void tim20_up_handler(void)__attribute__((weak, alias("default_handler")));
void tim20_trg_com_handler(void)__attribute__((weak, alias("default_handler")));
void tim20_cc_handler(void)__attribute__((weak, alias("default_handler")));
void fpu_handler(void)__attribute__((weak, alias("default_handler")));
void spi4_handler(void)__attribute__((weak, alias("default_handler")));

uint32_t isr_vector[ISR_VECTOR_SIZE] __attribute__((section(".vectors"))) = {
    STACK_PTR_INIT_ADD,
    (uint32_t)&reset_handler,
    (uint32_t)&nmi_handler,
    (uint32_t)&hard_fault_handler,
    (uint32_t)&mem_manage_handler,
    (uint32_t)&bus_fault_handler,
    (uint32_t)&usage_handler,
    0,
    0,
    0,
    0,
    (uint32_t)&svcall_handler,
    (uint32_t)&pendsv_handler,
    (uint32_t)&systick_handler,
    (uint32_t)&wwdg_handler,
    (uint32_t)&pvd_handler,
    (uint32_t)&tamper_stamp_handler,
    (uint32_t)&rtc_wkup_handler,
    (uint32_t)&flash_handler,
    (uint32_t)&rcc_handler,
    (uint32_t)&exti0_handler,
    (uint32_t)&exti1_handler,
    (uint32_t)&exti2_ts_handler,
    (uint32_t)&exti3_handler,
    (uint32_t)&exti4_handler,
    (uint32_t)&dma1_channel1_handler,
    (uint32_t)&dma1_channel2_handler,
    (uint32_t)&dma1_channel3_handler,
    (uint32_t)&dma1_channel4_handler,
    (uint32_t)&dma1_channel5_handler,
    (uint32_t)&dma1_channel6_handler,
    (uint32_t)&dma1_channel7_handler,
    (uint32_t)&adc1_2_handler,
    (uint32_t)&usb_hp_can_tx_handler,
    (uint32_t)&usb_lp_can_rx0_handler,
    (uint32_t)&can_rx1_handler,
    (uint32_t)&can_sce_handler,
    (uint32_t)&exti9_5_handler,
    (uint32_t)&tim1_brk_tim15_handler,
    (uint32_t)&tim1_up_tim16_handler,
    (uint32_t)&tim1_trg_com_tim17_handler,
    (uint32_t)&tim1_cc_handler,
    (uint32_t)&tim2_handler,
    (uint32_t)&tim3_handler,
    (uint32_t)&tim4_handler,
    (uint32_t)&i2c1_ev_handler,
    (uint32_t)&i2c1_er_handler,
    (uint32_t)&i2c2_ev_handler,
    (uint32_t)&i2c2_er_handler,
    (uint32_t)&spi1_handler,
    (uint32_t)&spi2_handler,
    (uint32_t)&usart1_handler,
    (uint32_t)&usart2_handler,
    (uint32_t)&usart3_handler,
    (uint32_t)&exti15_10_handler,
    (uint32_t)&rtc_alarm_handler,
    (uint32_t)&usb_wakeup_handler,
    (uint32_t)&tim8_brk_handler,
    (uint32_t)&tim8_up_handler,
    (uint32_t)&tim8_trg_com_handler,
    (uint32_t)&tim8_cc_handler,
    (uint32_t)&adc3_handler,
    (uint32_t)&fmc_handler,
    0,
    0,
    (uint32_t)&spi3_handler,
    (uint32_t)&uart4_handler,
    (uint32_t)&uart5_handler,
    (uint32_t)&tim6_dac_handler,
    (uint32_t)&tim7_handler,
    (uint32_t)&dma2_channel1_handler,
    (uint32_t)&dma2_channel2_handler,
    (uint32_t)&dma2_channel3_handler,
    (uint32_t)&dma2_channel4_handler
    (uint32_t)&dma2_channel5_handler,
    (uint32_t)&adc4_handler,
    0,
    0,
    (uint32_t)&comp123_handler,
    (uint32_t)&comp456_handler,
    (uint32_t)&comp7_handler,
    0,
    0,
    0,
    0,
    0,
    (uint32_t)&i2c3_ev_handler,
    (uint32_t)&i2c3_er_handler,
    (uint32_t)&usb_hp_handler,
    (uint32_t)&usb_lp_handler,
    (uint32_t)&usb_wakeup_rmp_handler,
    (uint32_t)&tim20_brk_handler,
    (uint32_t)&tim20_up_handler,
    (uint32_t)&tim20_trg_com_handler,
    (uint32_t)&tim20_cc_handler,
    (uint32_t)&fpu_handler,
    0,
    0,
    (uint32_t)&spi4_handler,
}

// 16 standard and 74 STM32-specific handlers
__attribute__((section(".vectors"))) void (*const tab[16+74])(void) = {
    _estack, _reset
};