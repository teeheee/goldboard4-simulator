#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "sim_avr.h"

typedef struct pwm_info_t{
  int pin;
  uint64_t last_call;
  uint64_t low_time;
  uint64_t high_time;
  int last_pin_state;
  double pwm_value;
  avr_t* avr;
} pwm_info_t;

void init_pwm(avr_t *avr, pwm_info_t* pwm_info, char port, int pin);
double get_pwm_value(pwm_info_t* pwm_info);

#endif
