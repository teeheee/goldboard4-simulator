#ifndef _PCF8674_H_
#define _PCF8674_H_

#include "i2c_device.h"

class pcf8574 : public i2c_device{
public:
  pcf8574() : i2c_device(0x70, "pcf8574"){
    pin_state = 0;
  }
  pcf8574(int address) : i2c_device(address, "pcf8574"){
      pin_state = 0;
  }
  void set_config(Json& config){
    address = (int)config["address"];
  }
  Json get_state(){
    Json state;
    state["type"] = "PCF8574";
    state["pin_state"] = (int)pin_state;
    return state;
  }
  void set_state(Json& data){
      pin_state = (int)data["pin_state"];
  }
  uint8_t read(){
    return pin_state;
  }
  void write(uint8_t data){
    pin_state = data;
  }
  uint8_t pin_state;
};

#endif
