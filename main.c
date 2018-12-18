// Bare metal blinking on STM32F1 BluePill
// TRM: https://www.st.com/resource/en/reference_manual/cd00171190.pdf

#include <stdint.h>

#define LED_PIN 13  // PC13

#define GPIOA ((volatile uint32_t *) 0x40010800)
#define GPIOB ((volatile uint32_t *) 0x4001c000)
#define GPIOC ((volatile uint32_t *) 0x40011000)
enum { CRL, CRH, IDR, ODR, BSRR, BRR, LCKR };
enum { MODE_OUT_2 = 2, PULL_UP_DOWN = 0, OPEN_DRAIN = 4 };

#define RCC ((volatile uint32_t *) 0x40021000)
enum { CR, CFGR, CIR, APB2RSTR, APB1RSTR, AHBENR, APB2ENR, APB1ENR, BDCR, CSR };

static void delay(uint32_t n) {
  volatile uint32_t i = n;
  while (i > 0) i--;
}

static void set_gpio_mode(volatile uint32_t *gpio, int pin, uint32_t mode) {
  uint32_t shift = (pin - 8) * 4;
  gpio[CRH] = (gpio[CRH] & ~(0xf << shift)) | (mode << shift);
}

int main(void) {
  RCC[APB2ENR] |= 0x10;  // Start clock on GPIOC
  set_gpio_mode(GPIOC, LED_PIN, MODE_OUT_2 | OPEN_DRAIN);

  for (;;) {
    GPIOC[BSRR] |= 1 << LED_PIN;
    delay(1 << 18);
    GPIOC[BSRR] |= 1 << (LED_PIN + 16);
    delay(1 << 18);
  }

  return 0;
}
