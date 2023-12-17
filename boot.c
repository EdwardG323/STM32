#include <inttypes.h>

extern void _estack(void);

#define SRAM_START (0x20000000u)
#define SRAM_SIZE (32u * 1024u)
#define SRAM_END (SRAM_START + SRAM_SIZE)
#define STACK_PTR_INIT_ADD (SRAM_END)

// OR

// #define STACK_PTR_INIT_ADD &_estack

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

// // 16 standard and 74 STM32-specific handlers
// __attribute__((section(".vectors"))) void (*const tab[16+74])(void) = {
//     _estack, _reset
// };

// Startup code
__attribute__((naked, noreturn)) void reset_handler(void){
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