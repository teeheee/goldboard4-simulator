#ifndef _CMPS11_H_
#define _CMPS11_H_

#include "i2c_register.h"

class cmps11 : public i2c_register<10>{
public:
   cmps11() : i2c_register(0xC0, "CMPS11"){
     angle = 0;
   }
   void set_config(Json& data){
     //TODO imptement this
   }
   void set_state(Json &data){
     angle = data["angle"];
     set_byte(1, (uint8_t)angle);
   }
   Json get_state(){
     Json state;
     state["type"] = "CMPS11";
     state["angle"] = angle;
     return state;
   }
   int angle;
};

#endif
