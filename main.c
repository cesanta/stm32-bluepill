// Copyright (c) 2018 Cesanta Software Limited
// All rights reserved
//
// Bare metal BluePill controlled by the Cloud Communication Module (CCM)

#include "stm32f1.h"

void uart_write(struct uart *uart, const void *ptr, int size) {
  for (int i = 0; i < size; i++) {
    uart->DR = ((unsigned char *) ptr)[i];  // Write next byte
    do {
    } while ((uart->SR & (1 << 7)) == 0);  // TXE is clear
  }
}

int uart_read(void) {
  // if ((USART[ISR] & (1 << U_ORE))) USART[ICR] |= 1 << U_ORE;  // Clear
  // overrun if ((USART[ISR] & (1 << U_RXNE))) return USART[RDR] & 0xff;
  return -1;
}

int main(void) {
  RCC->APB2ENR |= 1 << 2;   // GPIOA, for UART TX/RX on PA9 and PA10
  RCC->APB2ENR |= 1 << 4;   // GPIOC, for LED on PC13
  RCC->APB2ENR |= 1 << 14;  // enable clock on USART1

  SET_PIN_MODE(GPIOC, LED_PIN, 0b0110);  // open drain, output 2

  SET_PIN_MODE(GPIOA, TX_PIN, 2);                // TX pin mode = AF
  SET_PIN_MODE(GPIOA, RX_PIN, 2);                // RX pin mode = AF
  UART1->CR1 = (1 << 13) | (1 << 2) | (1 << 3);  // Enable USART1
  UART1->BRR = 0x45;                             // Set baud rate
  // UART1->CR1 = 0x200c;

  volatile int count = 0, on = 0, period = 60000;
  for (;;) {
    if (++count < period) continue;
    count = 0;
    on = !on;
    GPIOC->BSRR |= 1 << (LED_PIN + (on ? 0 : 16));
    uart_write(UART1, "{\"a\":1}", 8);
  }
  return 0;
}
