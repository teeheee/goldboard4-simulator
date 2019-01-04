#ifndef _USRING_H_
#define _USRING_H_

#include "i2c_register.h"

class usring : public i2c_register<10>{
public:
   usring(struct avr_t* avr) : i2c_register(avr, 0x68, "PIXY"){
   };
   void set_value(int angle){
      set_byte(1, (uint8_t)angle);
   };
};

#endif
