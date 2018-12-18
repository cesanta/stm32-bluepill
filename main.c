// Bare metal blinking on STM32F1
// TRM: https://www.st.com/resource/en/reference_manual/cd00171190.pdf

#define LED_PIN 13

#define GPIOA ((volatile unsigned *) 0x40010800)
#define GPIOB ((volatile unsigned *) 0x4001c000)
#define GPIOC ((volatile unsigned *) 0x40011000)
enum { CRL, CRH, IDR, ODR, BSRR, BRR, LCKR };
enum { MODE_OUT_2 = 2, PULL_UP_DOWN = 0, OPEN_DRAIN = 4 };

#define RCC ((volatile unsigned *) 0x40021000)
enum { CR, CFGR, CIR, APB2RSTR, APB1RSTR, AHBENR, APB2ENR, APB1ENR, BDCR, CSR };

static void delay(unsigned n) {
  volatile unsigned i = n;
  while (i > 0) i--;
}

int main(void) {
  RCC[APB2ENR] |= 0x10;  // Start clock on GPIOC

  unsigned shift = (LED_PIN - 8) * 4;
  unsigned conf = GPIOC[CRH] & ~(0xf << shift);
  conf |= (MODE_OUT_2 | OPEN_DRAIN) << shift;
  GPIOC[CRH] = conf;

  for (;;) {
    GPIOC[BSRR] |= 1 << LED_PIN;
    delay(1 << 19);
    GPIOC[BSRR] |= 1 << (LED_PIN + 16);
    delay(1 << 19);
  }

  return 0;
}
