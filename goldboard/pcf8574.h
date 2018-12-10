#ifndef _PCF8674_H_
#define _PCF8674_H_

#include "sim_avr.h"
#include "i2c_sensor.h"

typedef struct pcf8574_t {
  i2c_sensor_t i2c_sensor;
} pcf8574_t;

void pcf8574_init(struct avr_t* avr, pcf8574_t* pcf, int adress);
int pcf8574_getValue(pcf8574_t* pcf);
void pcf8574_setValue(pcf8574_t* pcf, int data);

#endif
