// Copyright (c) 2018,2019,2020 Cesanta Software Limited
// All rights reserved

#include "stm32f1.h"
#include "mjson.h"

static int uart_write(const char *ptr, int size, void *userdata) {
  struct uart *uart = userdata;
  for (int i = 0; i < size; i++) {
    uart->DR = ((unsigned char *) ptr)[i];  // Write next byte
    while ((uart->SR & BIT(7)) == 0)        // Wait until byte is scheduled
      ;
  }
  return size;
}

static int blink_period = 150000;

static void set_cycles(struct jsonrpc_request *r) {
  double dv;
  if (mjson_get_number(r->params, r->params_len, "$.period", &dv)) {
    blink_period = dv;
    jsonrpc_return_success(r, NULL);  // Report success
  }  else {
    jsonrpc_return_error(r, JSONRPC_ERROR_BAD_PARAMS, "Expect period", NULL);
  }
}

int main(void) {
  INIT_MEMORY;

  RCC->APB2ENR |= BIT(4);                // GPIOC, for LED on PC13
  SET_PIN_MODE(GPIOC, LED_PIN, 0b0110);  // open drain, output 2

  RCC->APB2ENR |= BIT(2);   // GPIOA, for USART1 TX/RX on PA9 and PA10
  RCC->APB2ENR |= BIT(14);  // enable clock on USART1
  SET_PIN_MODE(GPIOA, TX_PIN, 0b1010);     // TX pin mode = AF, push/pull
  SET_PIN_MODE(GPIOA, RX_PIN, 0b0100);     // RX pin mode = input, floating
  UART1->BRR = 0x45;                       // Set baud rate, TRM 27.3.4
  UART1->CR1 = BIT(13) | BIT(2) | BIT(3);  // Enable USART1

  jsonrpc_init(NULL, NULL);                       // Init JSON-RPC lib
  jsonrpc_export("SetCycles", set_cycles, NULL);  // Export custom function

  volatile int count = 0, led_on = 0;
  for (;;) {
    if (UART_HAS_DATA(UART1)) {
      jsonrpc_process_byte(UART_READ(UART1), uart_write, UART1);
    }
    if (++count < blink_period) continue;
    count = 0;
    led_on = !led_on;
    GPIOC->BSRR |= BIT(LED_PIN + (led_on ? 0 : 16));
  }
  return 0;
}

void *_sbrk() {
  return 0;
}
