#ifndef _PCF8674_H_
#define _PCF8674_H_

#include "i2c_device.h"

typedef struct pcf8574_t {
  i2c_device_t i2c_device;
  uint8_t output_port;
  uint8_t input_port;
} pcf8574_t;

void pcf8574_init(struct avr_t* avr, pcf8574_t* pcf, uint8_t adress);
uint8_t pcf8574_getValue(pcf8574_t* pcf);
void pcf8574_setValue(pcf8574_t* pcf, uint8_t data);

#endif
