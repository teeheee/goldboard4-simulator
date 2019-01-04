#include "goldboard.h"

goldboard::goldboard(struct avr_t* aavr)
        : pcf_motor(aavr, 0x70),
          pcf_digital(aavr, 0x7E),
          uart_obj(aavr)
{
  avr = aavr;
  init_pwm(avr, &motor_pwm[0], 'B', 0);
  init_pwm(avr, &motor_pwm[1], 'B', 1);
  init_pwm(avr, &motor_pwm[2], 'B', 2);
  init_pwm(avr, &motor_pwm[3], 'B', 3);
  init_pwm(avr, &power_pwm[0], 'B', 4);
  init_pwm(avr, &power_pwm[1], 'B', 5);
}

goldboard::~goldboard(){

}

//interface function
int goldboard::get_led_status(int id){

}

double goldboard::get_motor_speed(int id){
  if(pcf_motor.read_pins() & (1 << id))
    return get_pwm_value(&motor_pwm[id]);
  else if(pcf_motor.read_pins() & (1 << id+1))
    return -get_pwm_value(&motor_pwm[id]);
  else
    return 0;
}

double goldboard::get_power_pin(int id){
  return power_pwm[id].pwm_value;
}

std::string goldboard::get_serial_data(){
  if(uart_obj.is_empty())
    return std::string("");
  else
    return std::string(1,uart_obj.read_char());
}

void goldboard::set_button_status(int id, int state){

}
