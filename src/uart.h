#ifndef _UART_H_
#define _UART_H_

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "avr_uart.h"
#include "sim_avr.h"
#include "fifo_declare.h"

#define SERIAL_BUFFER_SIZE 1024

DECLARE_FIFO(uint8_t,uart_stream,SERIAL_BUFFER_SIZE);

typedef struct uart_t{
  avr_irq_t* irq;
  uart_stream_t in_fifo;
} uart_t;

void uart_init(avr_t* avr, uart_t* uart);
int uart_is_empty(uart_t* uart);
char uart_getc(uart_t* uart);

#endif
