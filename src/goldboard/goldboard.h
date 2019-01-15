#ifndef _GOLDBOARD_H_
#define _GOLDBOARD_H_

#include "i2c_device.h"
#include "pcf8574.h"
extern "C"{
#include "pwm.h"
}
#include "uart.h"

class goldboard{
public:
  goldboard(struct avr_t* avr);
  ~goldboard();
  void add_i2c_device(i2c_device &device);
  int run(int ms);
  void set_state(Json &data);
  Json get_state();

private:
  //interface function
  int get_led_status(int id);
  double get_motor_speed(int id);
  double get_power_pin(int id);
  std::string get_serial_data();
  int get_time();

  void set_pin_state(int id, int state);
  void set_analog_state(int id, int state);
  void set_button_status(int id, int state);

  struct avr_t* avr;
  pwm_info_t motor_pwm[4];
  pwm_info_t power_pwm[2];
  uart uart_obj;
  pcf8574 pcf_motor;
  pcf8574 pcf_digital;
  int button_state[2];
};

#endif
