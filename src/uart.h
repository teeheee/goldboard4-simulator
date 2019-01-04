#ifndef _UART_H_
#define _UART_H_

#include <queue>
#include "avr_uart.h"
#include "sim_avr.h"

class uart{
public:
  uart(struct avr_t* avr);
  ~uart();
  int is_empty();
  char read_char();
  std::queue<char> fifo_buffer;
private:
  avr_irq_t* irq;
};


#endif
