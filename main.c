// Copyright (c) 2018 Cesanta Software Limited
// All rights reserved
//
// Bare metal BluePill controlled by the Cloud Communication Module (CCM)

#include "stm32f1.h"

void uart_write(struct uart *uart, const void *ptr, int size) {
  for (int i = 0; i < size; i++) {
    uart->DR = ((unsigned char *) ptr)[i];  // Write next byte
    while ((uart->SR & BIT(7)) == 0)        // Wait until byte is scheduled
      ;
  }
}

// static int uart_read(void) {
//   // if ((USART[ISR] & (1 << U_ORE))) USART[ICR] |= 1 << U_ORE;  // Clear
//   // overrun if ((USART[ISR] & (1 << U_RXNE))) return USART[RDR] & 0xff;
//   return -1;
// }

int main(void) {
  RCC->APB2ENR |= BIT(4);                // GPIOC, for LED on PC13
  SET_PIN_MODE(GPIOC, LED_PIN, 0b0110);  // open drain, output 2

  RCC->APB2ENR |= BIT(2);   // GPIOA, for USART1 TX/RX on PA9 and PA10
  RCC->APB2ENR |= BIT(14);  // enable clock on USART1
  SET_PIN_MODE(GPIOA, TX_PIN, 0b1010);     // TX pin mode = AF, push/pull
  SET_PIN_MODE(GPIOA, RX_PIN, 0b0100);     // RX pin mode = input, floating
  UART1->BRR = 0x270;                      // Set baud rate, TRM 27.3.4
  UART1->CR1 = BIT(13) | BIT(2) | BIT(3);  // Enable USART1

  volatile int count = 0, on = 0, period = 50000;
  for (;;) {
    if (++count < period) continue;
    count = 0;
    on = !on;
    GPIOC->BSRR |= BIT(LED_PIN + (on ? 0 : 16));
    uart_write(UART1, "{\"a\":1}\n", 8);
  }
  return 0;
}
