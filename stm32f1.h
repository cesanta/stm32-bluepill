// Copyright (c) 2018 Cesanta Software Limited
// All rights reserved
//
// TRM: https://www.st.com/resource/en/reference_manual/cd00171190.pdf

#include <stdint.h>
#include <string.h>

#define LED_PIN 13  // PC13
#define TX_PIN 9    // PA9
#define RX_PIN 10   // PA10

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
// enum { MODE_INPUT = 0, MODE_OUTPUT_1, MODE_OUTPUT_2, MODE_OUTPUT_3 };

// UART registers, TRM section 27.6
struct uart {
  volatile uint32_t SR, DR, BRR, CR1, CR2, CR3, GTPR;
};
#define UART1 ((struct uart *) 0x40013800)
#define UART2 ((struct uart *) 0x40004400)
#define UART3 ((struct uart *) 0x40004800)
#define UART4 ((struct uart *) 0x40004c00)
#define UART5 ((struct uart *) 0x40005000)

#define BIT(x) ((uint32_t) 1 << (x))

#define SET_REG(reg, clear_mask, set_mask) \
  do {                                     \
    reg &= ~(clear_mask);                  \
    reg |= (set_mask);                     \
  } while (0)

#define SET_PIN_MODE(gpio, pin, mode)                  \
  do {                                                 \
    uint32_t shift = ((pin) -8) * 4;                   \
    SET_REG((gpio)->CRH, 0xf << shift, mode << shift); \
  } while (0)

#define UART_HAS_DATA(u) ((u)->SR & BIT(5))  // RXNE
#define UART_READ(u) ((u)->DR & 255)

#define INIT_MEMORY                                                      \
  do {                                                                   \
    extern uint32_t _bss_start, _bss_end;                                \
    extern uint32_t _data_start, _data_end, _data_flash_start;           \
    memset(&_bss_start, 0, ((char *) &_bss_end - (char *) &_bss_start)); \
    memcpy(&_data_start, &_data_flash_start,                             \
           ((char *) &_data_end - (char *) &_data_start));               \
  } while (0)
