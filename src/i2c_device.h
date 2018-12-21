#ifndef __I2C_device_H___
#define __I2C_device_H___

#include <stddef.h>

#include "sim_avr.h"
#include "sim_irq.h"
#include "avr_twi.h"

typedef struct i2c_device_t i2c_device_t;

typedef uint8_t (*i2c_read_callback_t)(i2c_device_t* device); 
typedef void (*i2c_write_callback_t)(i2c_device_t* device, uint8_t data); 

typedef struct i2c_device_t {
	// configuration parameters
	uint8_t addr_base;
	uint8_t addr_mask;

	// i2c contoll variables
	void* param;
	avr_irq_t *	irq;		// irq list
	uint8_t selected;		// selected address (always a base adress)

	// callbalck functions for read and write request
	i2c_read_callback_t read_callback;
	i2c_write_callback_t write_callback;
} i2c_device_t;

/*
 * The address is the TWI/i2c address base, for example 0xa0 -- the 7 MSB are
 * relevant, the bit zero is always meant as the "read write" bit.
 * The "mask" parameter specifies which bits should be matched as a slave;
 * if you want to have a peripheral that handle read and write, use '1'; if you
 * want to also match several addresses on the bus, specify these bits on the
 * mask too.
 * Example:
 * Address 0xa1 mask 0x00 will match address 0xa0 in READ only
 * Address 0xa0 mask 0x01 will match address 0xa0 in read AND write mode
 * Address 0xa0 mask 0x03 will match 0xa0 0xa2 in read and write mode
 *
 */
void
i2c_device_init(struct avr_t * avr,
		i2c_device_t * p,
		uint8_t addr,
		uint8_t mask,
		i2c_read_callback_t read_callback,
		i2c_write_callback_t write_callback );


/*
 * Attach the device to the AVR's TWI master code,
 * pass AVR_IOCTL_TWI_GETIRQ(0) for example as i2c_irq_base
 */
void
i2c_device_attach(struct avr_t * avr,
		  i2c_device_t * p,
		  uint32_t i2c_irq_base );


#endif /* __i2c_device_H___ */
