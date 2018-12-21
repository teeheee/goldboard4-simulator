#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "sim_avr.h"
#include "pcf8574.h"


typedef struct pwm_info{
  int pin;
  uint64_t last_call;
  uint64_t low_time;
  uint64_t high_time;
  int last_pin_state;
  double pwm_value;
  avr_t* avr;
} pwm_info;


void init_motor(avr_t *avr, pcf8574_t* pcf);
double get_pwm_value(int id);

#endif
