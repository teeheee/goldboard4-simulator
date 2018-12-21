
#include "i2c_device.h"

/*
 * called when a RESET signal is sent
 */
static void i2c_device_hook(
		struct avr_irq_t * irq,
		uint32_t value,
		void * param)
{
	i2c_device_t * p = (i2c_device_t*)param;
	avr_twi_msg_irq_t v;
	v.u.v = value;

	/*
	 * If we receive a STOP, check it was meant to us, and reset the transaction
	 */
	if (v.u.twi.msg & TWI_COND_STOP) {
		p->selected = 0;
	}
	/*
	 * if we receive a start, reset status, check if the slave address is
	 * meant to be us, and if so reply with an ACK bit
	 */
	if (v.u.twi.msg & TWI_COND_START) {
		p->selected = 0;
		if ((p->addr_base & ~p->addr_mask) == (v.u.twi.addr & ~p->addr_mask)) {
			// it's us !
			p->selected = v.u.twi.addr;
			avr_raise_irq(p->irq + TWI_IRQ_INPUT,
				      avr_twi_irq_msg(TWI_COND_ACK, p->selected, 1));
		}
	}
	/*
	 * If it's a data transaction, first check it is meant to be us (we
	 * received the correct address and are selected)
	 */
	if (p->selected) {
		/*
		 * This is a write transaction
		 */
		if (v.u.twi.msg & TWI_COND_WRITE) {
			avr_raise_irq(p->irq + TWI_IRQ_INPUT,
				      avr_twi_irq_msg(TWI_COND_ACK, p->selected, 1));
			p->write_callback(p, v.u.twi.data);
		}
		/*
		 * It's a read transaction
		 */
		if (v.u.twi.msg & TWI_COND_READ) {
			uint8_t data = p->read_callback(p);
			avr_raise_irq(p->irq + TWI_IRQ_INPUT,
				      avr_twi_irq_msg(TWI_COND_READ, p->selected, data));
		}
	}
}

void
i2c_device_init(struct avr_t * avr,
		i2c_device_t * p,
		uint8_t addr,
		uint8_t mask,
		i2c_read_callback_t read_callback,
		i2c_write_callback_t write_callback )
{
	p->addr_base = addr;
	p->addr_mask = mask;
	p->read_callback = read_callback;
	p->write_callback = write_callback;

	p->irq = avr_alloc_irq(&avr->irq_pool, 0, 2, NULL);
	avr_irq_register_notify(p->irq + TWI_IRQ_OUTPUT, i2c_device_hook, p);
}

void
i2c_device_attach(struct avr_t * avr,
		  i2c_device_t * p,
		  uint32_t i2c_irq_base )
{
	// "connect" the IRQs of the device to the TWI/i2c master of the AVR
	avr_connect_irq(
		p->irq + TWI_IRQ_INPUT,
		avr_io_getirq(avr, i2c_irq_base, TWI_IRQ_INPUT));
	avr_connect_irq(
		avr_io_getirq(avr, i2c_irq_base, TWI_IRQ_OUTPUT),
		p->irq + TWI_IRQ_OUTPUT );
}
