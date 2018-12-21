// Copyright (c) 2018 Cesanta Software Limited
// All rights reserved
//
// Bare metal BluePill controlled by the Cloud Communication Module (CCM)

#include "stm32f1.h"

#define MJSON_IMPLEMENT_STRTOD 1
#include "../mjson/mjson.h"

static int uart_write(const char *ptr, int size, void *userdata) {
  struct uart *uart = userdata;
  for (int i = 0; i < size; i++) {
    uart->DR = ((unsigned char *) ptr)[i];  // Write next byte
    while ((uart->SR & BIT(7)) == 0)        // Wait until byte is scheduled
      ;
  }
  return size;
}

void *_sbrk() {
  return 0;
}

void init_data_section(void) {
  extern uint32_t _bss_start, _bss_end;
  extern uint32_t _data_start, _data_end, _data_flash_start;
  memset(&_bss_start, 0, ((char *) &_bss_end - (char *) &_bss_start));
  memcpy(&_data_start, &_data_flash_start,
         ((char *) &_data_end - (char *) &_data_start));
}

int main(void) {
  init_data_section();

  RCC->APB2ENR |= BIT(4);                // GPIOC, for LED on PC13
  SET_PIN_MODE(GPIOC, LED_PIN, 0b0110);  // open drain, output 2

  RCC->APB2ENR |= BIT(2);   // GPIOA, for USART1 TX/RX on PA9 and PA10
  RCC->APB2ENR |= BIT(14);  // enable clock on USART1
  SET_PIN_MODE(GPIOA, TX_PIN, 0b1010);     // TX pin mode = AF, push/pull
  SET_PIN_MODE(GPIOA, RX_PIN, 0b0100);     // RX pin mode = input, floating
  UART1->BRR = 0x45;                       // Set baud rate, TRM 27.3.4
  UART1->CR1 = BIT(13) | BIT(2) | BIT(3);  // Enable USART1

  jsonrpc_init(uart_write, NULL, UART1, "1.0");

  volatile int count = 0, led_on = 0, period = 50000;
  for (;;) {
    if (UART_HAS_DATA(UART1)) jsonrpc_process_byte(UART_READ(UART1));
    if (++count < period) continue;
    count = 0;
    led_on = !led_on;
    GPIOC->BSRR |= BIT(LED_PIN + (led_on ? 0 : 16));
  }
  return 0;
}
