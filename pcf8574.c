#include "pcf8574.h"

static uint8_t i2c_read_callback(i2c_device_t* device){
	pcf8574_t* p = (pcf8574_t*)(device->param);
	return p->output_port;
}

static void i2c_write_callback(i2c_device_t* device, uint8_t data){
	pcf8574_t* p = (pcf8574_t*)device->param;
	p->input_port = data;
}

void pcf8574_init(struct avr_t* avr, pcf8574_t* pcf, uint8_t adress){
	pcf->i2c_device.param = (void*)pcf;
	i2c_device_init(avr, &pcf->i2c_device, adress, 1, &i2c_read_callback, &i2c_write_callback);
	i2c_device_attach(avr, &pcf->i2c_device, AVR_IOCTL_TWI_GETIRQ(0));
}


uint8_t pcf8574_getValue(pcf8574_t* pcf){
  	return pcf->input_port;
}

void pcf8574_setValue(pcf8574_t* pcf, uint8_t data){
  	pcf->output_port = data;
}
