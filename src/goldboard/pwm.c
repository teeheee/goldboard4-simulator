#include "pwm.h"
#include "sim_avr.h"
#include "avr_ioport.h"
#include <stdio.h>


void pwm_callback(struct avr_irq_t * avr, unsigned int value, void * param){
  pwm_info_t* p = (pwm_info_t*)param;
  int pin_state = value & (1 << p->pin);
  uint64_t cycle = p->avr->cycle;
  if(pin_state && !p->last_pin_state) //rising edge
  {
    p->low_time = cycle - p->last_call;
    p->last_call = cycle;
    p->last_pin_state = pin_state;
  }
  else if(!pin_state && p->last_pin_state) //falling edge
  {
    p->high_time = cycle - p->last_call;
    p->last_call = cycle;
    p->last_pin_state = pin_state;
    p->pwm_value =   (double)p->high_time/(p->high_time+p->low_time);
  }
}

void init_pwm(avr_t *avr, pwm_info_t* pwm_info, char port, int pin){
  pwm_info->last_call = 0;
  pwm_info->pin = pin;
  pwm_info->pwm_value = 0;
  pwm_info->last_pin_state = 0;
  pwm_info->avr = avr;
  avr_irq_register_notify(
          avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ(port), pin),
    			pwm_callback,
          pwm_info);
}


double get_pwm_value(pwm_info_t* pwm_info){
  return pwm_info->pwm_value;
}
