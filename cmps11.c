#include "cmps11.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sim_avr.h"
#include "avr_twi.h"
#include "i2c_sensor.h"

void cmps11_init(struct avr_t* avr, cmps11_t* cmps, int adress){
	i2c_sensor_init(avr, &cmps->i2c_sensor, adress, 1, 0, 1024);
	i2c_sensor_attach(avr, &cmps->i2c_sensor, AVR_IOCTL_TWI_GETIRQ(0));
}


void cmps11_setValue(cmps11_t* cmps, int data){
  cmps->i2c_sensor.output[1] = (uint8_t)(data*255.0/360.0);
  cmps->i2c_sensor.output[2] = data;
  cmps->i2c_sensor.output[3] = data>>8;
}
