#include "pcf8574.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sim_avr.h"
#include "avr_twi.h"
#include "i2c_sensor.h"

void pcf8574_init(struct avr_t* avr, pcf8574_t* pcf, int adress){
	i2c_sensor_init(avr, &pcf->i2c_sensor, adress, 1, 0, 1024);
	i2c_sensor_attach(avr, &pcf->i2c_sensor, AVR_IOCTL_TWI_GETIRQ(0));
}


int pcf8574_getValue(pcf8574_t* pcf){
  return pcf->i2c_sensor.input[0];
}

void pcf8574_setValue(pcf8574_t* pcf, int data){
  pcf->i2c_sensor.output[0] = data;
}
