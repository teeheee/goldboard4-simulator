#ifndef _CMPS11_H_
#define _CMPS11_H_

#include "i2c_register.h"

class cmps11 : public i2c_register<10>{
public:
   cmps11(struct avr_t* avr) : i2c_register(avr, 0xC0, "CMPS11"){
   };
   void set_value(int angle){
      set_byte(1, (uint8_t)angle);
   };
};

#endif
