#ifndef _ATMEGA32_H_
#define _ATMEGA32_H_

class atmega32{
public:
  atmega32(const char* firmware);
  struct avr_t* get_simavr_avr();
private:
  struct avr_t* avr;
};

#endif
