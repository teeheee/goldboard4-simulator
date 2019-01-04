#include "i2c_device.h"

static void i2c_device_hook(struct avr_irq_t * irq, uint32_t value, void* adevice){
  i2c_device* p = (i2c_device*)adevice;
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
		if (p->address == v.u.twi.addr || p->address == v.u.twi.addr-1){
			// it's us !
			p->selected = v.u.twi.addr;
			avr_raise_irq(p->irq_list + TWI_IRQ_INPUT,
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
			avr_raise_irq(p->irq_list + TWI_IRQ_INPUT,
				      avr_twi_irq_msg(TWI_COND_ACK, p->selected, 1));
			p->write(v.u.twi.data);
		}
		/*
		 * It's a read transaction
		 */
		if (v.u.twi.msg & TWI_COND_READ) {
			uint8_t data = p->read();
			avr_raise_irq(p->irq_list + TWI_IRQ_INPUT,
				      avr_twi_irq_msg(TWI_COND_READ, p->selected, data));
		}
	}
}


i2c_device::i2c_device(uint8_t aaddress, const char* name){
  address = aaddress;
  selected = 0;
  c_names[0] = (std::string(name) + "_in").c_str();
  c_names[1] = (std::string(name) + "_out").c_str();
}

void i2c_device::attach(struct avr_t * avr){
  irq_list = avr_alloc_irq(&avr->irq_pool, 0, 2, c_names);
  avr_irq_register_notify(irq_list + TWI_IRQ_OUTPUT, i2c_device_hook, (void*)this);
  // "connect" the IRQs of the device to the TWI/i2c master of the AVR
	avr_connect_irq(irq_list + TWI_IRQ_INPUT,
		              avr_io_getirq(avr, AVR_IOCTL_TWI_GETIRQ(0), TWI_IRQ_INPUT));
	avr_connect_irq(avr_io_getirq(avr, AVR_IOCTL_TWI_GETIRQ(0), TWI_IRQ_OUTPUT),
                  irq_list + TWI_IRQ_OUTPUT );
}

i2c_device::~i2c_device(){
  //TODO detatch irq
}
