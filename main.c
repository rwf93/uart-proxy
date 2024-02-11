#include "hardware/irq.h"
#include "hardware/regs/intctrl.h"
#include <stdio.h>
#include <string.h>

// #include <pico/stdio.h>

#include <hardware/gpio.h>
#include <pico/stdio.h>
#include <pico/stdio_uart.h>
#include <pico/stdlib.h>
#include <pico/multicore.h>
#include <pico.h>
#include <pico/time.h>
#include <pico/bootrom.h>
#include <hardware/regs/resets.h>
#include <hardware/uart.h>

#define UART_TX_PIN 0
#define UART_RX_PIN 1

#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY    UART_PARITY_NONE

#define BAUD_RATE 115200

#define UART_ID uart0

void uart_rx_event() {
  while (uart_is_readable(UART_ID)) {
    char ch = uart_getc(UART_ID);
    printf("%c", ch);
  }
}

int main() { 
  stdio_usb_init(); // only allow usb for getlining data... 

  uart_init(UART_ID, 115200);  

  gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
  gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

  uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);
  uart_set_fifo_enabled(UART_ID, true); 

  irq_set_exclusive_handler(UART0_IRQ, uart_rx_event);
  irq_set_enabled(UART0_IRQ, true);

  uart_set_irq_enables(UART_ID, true, false);

  while (1) {
    while(uart_is_writable(UART_ID)) {
      char input = getchar();
      uart_putc_raw(UART_ID, input);
    } 
  }

  return 0;
}
