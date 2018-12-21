#include "motor.h"
#include "sim_avr.h"
#include "avr_ioport.h"
#include <stdio.h>

pwm_info motor_pwm_info[4];
pcf8574_t* motor_pcf;

void pwm_callback(struct avr_irq_t * avr, unsigned int value, void * param){
  pwm_info* motor_pwm = (pwm_info*)param;
  int pin_state = value & (1 << motor_pwm->pin);
  uint64_t cycle = motor_pwm->avr->cycle;
  if(pin_state && !motor_pwm->last_pin_state) //rising edge
  {
    motor_pwm->low_time = cycle - motor_pwm->last_call;
    motor_pwm->last_call = cycle;
    motor_pwm->last_pin_state = pin_state;
  }
  else if(!pin_state && motor_pwm->last_pin_state) //falling edge
  {
    motor_pwm->high_time = cycle - motor_pwm->last_call;
    motor_pwm->last_call = cycle;
    motor_pwm->last_pin_state = pin_state;
    motor_pwm->pwm_value =   (double)motor_pwm->high_time/(motor_pwm->high_time+motor_pwm->low_time);
  }
}

void init_motor(avr_t *avr, pcf8574_t* pcf){
  motor_pcf = pcf;
  for(int i = 0; i < 4; i++){
    motor_pwm_info[i].last_call = 0;
    motor_pwm_info[i].pin = i;
    motor_pwm_info[i].pwm_value = 0;
    motor_pwm_info[i].last_pin_state = 0;
    motor_pwm_info[i].avr = avr;
    avr_irq_register_notify(
          avr_io_getirq(avr, AVR_IOCTL_IOPORT_GETIRQ('B'), i),
    			pwm_callback,
          &motor_pwm_info[i]);
  }
}


double get_pwm_value(int id){
  uint8_t dir = pcf8574_getValue(motor_pcf);
  if(dir & (1 << (id*2)))
    return motor_pwm_info[id].pwm_value;
  if(dir & (1 << (id*2+1)))
    return -motor_pwm_info[id].pwm_value;
  return 0;
}
