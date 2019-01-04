#ifndef _I2C_DEVICE_H_
#define _I2C_DEVICE_H_


#include <string>
#include "sim_avr.h"
#include "sim_irq.h"
#include "avr_twi.h"

class i2c_device{
public:
  i2c_device(uint8_t address, const char* name);
  ~i2c_device();
  virtual uint8_t read() = 0;
  virtual void write(uint8_t data) = 0;
  void attach(struct avr_t * avr);

  uint8_t address;
  uint8_t selected;
  struct avr_t* avr;
  struct avr_irq_t*	irq_list;
  const char* c_names[2];
};

#endif
