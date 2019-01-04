#ifndef _PIXY_H_
#define _PIXY_H_

#include "i2c_register.h"

class pixy : public i2c_register<10>{
public:
   pixy(struct avr_t* avr) : i2c_register(avr, 0x54, "PIXY"){
   };
   void set_value(int angle){
      set_byte(1, (uint8_t)angle);
   };
};

#endif
