#include "uart.h"

#include <stdio.h>
#include "fifo_declare.h"


DEFINE_FIFO(uint8_t,uart_stream);

static void uart_in_hook(struct avr_irq_t * irq, uint32_t value, void * param)
{
	uart_t* p = (uart_t*)param;
	uart_stream_write(&p->in_fifo, value);
}


void uart_init(avr_t* avr, uart_t* uart){
	// disable the stdio dump, as we are sending binary there
	uint32_t f = 0;
	avr_ioctl(avr, AVR_IOCTL_UART_GET_FLAGS('0'), &f);
	f &= ~AVR_UART_FLAG_STDIO;
	avr_ioctl(avr, AVR_IOCTL_UART_SET_FLAGS('0'), &f);

  uart->irq = avr_io_getirq(avr, AVR_IOCTL_UART_GETIRQ('0'), UART_IRQ_OUTPUT);
	avr_irq_register_notify(uart->irq, uart_in_hook, uart);
}

int uart_is_empty(uart_t* uart){
  return uart_stream_isempty(&uart->in_fifo);
}

char uart_getc(uart_t* uart){
  return uart_stream_read(&uart->in_fifo);
}
