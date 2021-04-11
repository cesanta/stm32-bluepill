// Copyright (c) 2018-2021 Cesanta Software Limited
// All rights reserved

#include "stm32f1.h"

static int blink_period = 450000;

static void setup(void) {
  INIT_MEMORY;

  RCC->APB2ENR |= BIT(4);                // GPIOC, for LED on PC13
  SET_PIN_MODE(GPIOC, LED_PIN, 0b0110);  // open drain, output 2

  RCC->APB2ENR |= BIT(2);   // GPIOA, for USART1 TX/RX on PA9 and PA10
  RCC->APB2ENR |= BIT(14);  // enable clock on USART1
  SET_PIN_MODE(GPIOA, TX_PIN, 0b1010);     // TX pin mode = AF, push/pull
  SET_PIN_MODE(GPIOA, RX_PIN, 0b0100);     // RX pin mode = input, floating
  UART1->BRR = 0x45;                       // Set baud rate, TRM 27.3.4
  UART1->CR1 = BIT(13) | BIT(2) | BIT(3);  // Enable USART1
}

static void uart_putc(struct uart *uart, uint8_t byte) {
  uart->DR = byte;                  // Write next byte
  do {
  } while ((uart->SR & BIT(7)) == 0);  // Wait until byte is scheduled
}

int main(void) {
  setup();
  volatile int count = 0, led_on = 0;
  for (;;) {
    if (UART_HAS_DATA(UART1)) {
      int ch = UART_READ(UART1);
      if (ch >= '0' && ch <= '9') blink_period = (ch - '0' + 1) * 150000;
    }
    if (++count > blink_period) {
      count = 0;
      led_on = !led_on;
      GPIOC->BSRR |= BIT(LED_PIN + (led_on ? 0 : 16));
      uart_putc(UART1, led_on ? '1' : '0');
    }
  }
  return 0;
}

void *_sbrk() {
  return 0;
}
