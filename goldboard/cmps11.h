#ifndef _CMPS11_H_
#define _CMPS11_H_

#include "sim_avr.h"
#include "i2c_sensor.h"

typedef struct cmps11_t {
  i2c_sensor_t i2c_sensor;
} cmps11_t;

void cmps11_init(struct avr_t* avr, cmps11_t* cmps, int adress);
void cmps11_setValue(cmps11_t* cmps, int data);

#endif
