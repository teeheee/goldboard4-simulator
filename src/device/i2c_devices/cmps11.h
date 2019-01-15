#ifndef _CMPS11_H_
#define _CMPS11_H_

#include "i2c_register.h"

class cmps11 : public i2c_register<10>{
public:
   cmps11() : i2c_register(0xC0, "CMPS11"){};
   void set_state(Json &data){
     int angle = data["angle"];
     set_byte(1, (uint8_t)angle);
   }
   void set_value(int angle){
  };
};

#endif
