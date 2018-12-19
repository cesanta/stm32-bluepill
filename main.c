// Bare metal blinking on STM32F1 BluePill
// TRM: https://www.st.com/resource/en/reference_manual/cd00171190.pdf

typedef unsigned int uint32_t;

#define LED_PIN 13  // PC13

// RCC registers, TRM section 7.3
struct rcc {
  volatile uint32_t CR, CFGR, CIR, APB2RSTR, APB1RSTR, AHBENR;
  volatile uint32_t APB2ENR, APB1ENR, BDCR, CSR;
};
#define RCC ((struct rcc *) 0x40021000)

// GPIO registers, TRM section 9.2
struct gpio {
  volatile uint32_t CRL, CRH, IDR, ODR, BSRR, BRR, LCKR;
};
#define GPIOA ((struct gpio *) 0x40010800)
#define GPIOB ((struct gpio *) 0x4001c000)
#define GPIOC ((struct gpio *) 0x40011000)
enum { MODE_OUT_2 = 2, PULL_UP_DOWN = 0, OPEN_DRAIN = 4 };

static void delay(uint32_t n) {
  volatile uint32_t i = n;
  while (i > 0) i--;
}

static void set_gpio_mode(struct gpio *gpio, int pin, uint32_t mode) {
  uint32_t shift = (pin - 8) * 4;
  gpio->CRH = (gpio->CRH & ~(0xf << shift)) | (mode << shift);
}

int main(void) {
  RCC->APB2ENR |= 0x10;
  set_gpio_mode(GPIOC, LED_PIN, MODE_OUT_2 | OPEN_DRAIN);

  int cycles = 35000;
  for (;;) {
    GPIOC->BSRR |= 1 << LED_PIN;
    delay(cycles);
    GPIOC->BSRR |= 1 << (LED_PIN + 16);
    delay(cycles);
  }

  return 0;
}
