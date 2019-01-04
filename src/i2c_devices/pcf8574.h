#ifndef _PCF8674_H_
#define _PCF8674_H_

#include "i2c_device.h"

class pcf8574 : public i2c_device{
public:
  pcf8574(struct avr_t* avr, uint8_t address) : i2c_device(avr, address, "pcf8574"){
    pin_state = 0;
  };
  uint8_t read_pins(){
    return pin_state;
  };
  void write_pins(uint8_t data){
    pin_state = data;
  };
private:
  uint8_t read(){
    return pin_state;
  };
  void write(uint8_t data){
    pin_state = data;
  };
  uint8_t pin_state;
};

#endif
