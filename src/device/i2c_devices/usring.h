#ifndef _USRING_H_
#define _USRING_H_

#include "i2c_register.h"

class usring : public i2c_register<10>{
public:
   int analog_values[16];
   usring() : i2c_register(0x68, "usring"){
     for(int i = 0; i < 16; i++)
        analog_values[i] = 0;
   }
   void set_config(Json& data){
     //TODO imptement this
   }
   void set_state(Json& data){
     if((std::string)data["type"] == "USRING" && (int)data["id"] == get_id()){
       Json analog = data["analog"];
       for(int i = 0; i < 16; i++){
         analog_values[i] = analog[i];
       }
     }
   }
   Json get_state(){
     Json state;
     state["type"] = "USRING";
     Json json_analog;
     for(int i = 0; i < 16; i++)
        json_analog << (Json)analog_values[i];
     state["analog"] = json_analog;
     return state;
   }
};

#endif
