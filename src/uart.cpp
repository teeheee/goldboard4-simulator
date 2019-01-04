#include "uart.h"
#include <stdio.h>

static void uart_in_hook(struct avr_irq_t * irq, uint32_t value, void * param)
{
		uart* p = (uart*)param;
		p->fifo_buffer.push(value);
}

uart::uart(struct avr_t* avr){
		// disable the stdio dump, as we are sending binary there
		uint32_t f = 0;
		avr_ioctl(avr, AVR_IOCTL_UART_GET_FLAGS('0'), &f);
		f &= ~AVR_UART_FLAG_STDIO;
		avr_ioctl(avr, AVR_IOCTL_UART_SET_FLAGS('0'), &f);

	  irq = avr_io_getirq(avr, AVR_IOCTL_UART_GETIRQ('0'), UART_IRQ_OUTPUT);
		avr_irq_register_notify(irq, uart_in_hook, this);
}

uart::~uart(){

}

int uart::is_empty(){
		return fifo_buffer.empty();
}

char uart::read_char(){
	if(is_empty())
		return 0;
	char p = fifo_buffer.front();
	fifo_buffer.pop();
	return p;
}
