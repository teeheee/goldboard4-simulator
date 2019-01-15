#include "goldboard.h"

goldboard::goldboard(struct avr_t* aavr)
        : pcf_motor(0x70),
          pcf_digital(0x7E),
          uart_obj(aavr)
{
  printf("init goldboard\r\n");
  avr = aavr;
  init_pwm(avr, &motor_pwm[0], 'B', 0);
  init_pwm(avr, &motor_pwm[1], 'B', 1);
  init_pwm(avr, &motor_pwm[2], 'B', 2);
  init_pwm(avr, &motor_pwm[3], 'B', 3);
  init_pwm(avr, &power_pwm[0], 'B', 4);
  init_pwm(avr, &power_pwm[1], 'B', 5);
  add_i2c_device(pcf_motor);
  add_i2c_device(pcf_digital);
}

goldboard::~goldboard(){
  avr_terminate(avr);
}

void goldboard::add_i2c_device(i2c_device &device){
  device.attach(avr);
}

void goldboard::set_state(Json &data){
  set_button_status(0, data["buttons"][0]);
  set_button_status(1, data["buttons"][1]);
}

Json goldboard::get_state(){
  json11:Json json_data;

  json_data["time"] = get_time();

  json_data["goldboard"] = Json();
  json_data["goldboard"]["motor"] = Json::array();
  for(int i = 0; i < 4; i++)
    json_data["goldboard"]["motor"][i] = (int)get_motor_speed(i);

  json_data["goldboard"]["power_pin"] = Json::array();
  json_data["goldboard"]["power_pin"][0] = (int)get_power_pin(0);
  json_data["goldboard"]["power_pin"][1] = (int)get_power_pin(1);

  json_data["goldboard"]["led"] = Json::array();
  json_data["goldboard"]["led"][0] = (int)get_led_status(0);
  json_data["goldboard"]["led"][1] = (int)get_led_status(1);

  json_data["goldboard"]["serial"] = get_serial_data().c_str();

  return json_data;
}



//interface function
int goldboard::get_led_status(int id){
  return 0;
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
  std::string output = "";
  while(!uart_obj.is_empty()){
    output += std::string(1,uart_obj.read_char());
  }
  return output;
}

void goldboard::set_button_status(int id, int state){

}

int goldboard::run(int ms){
  unsigned long start_cycle = avr->cycle;
  while( avr->cycle-start_cycle < ms*16000)
  {
		int state = avr_run(avr);
		if (state == cpu_Done || state == cpu_Crashed)
			return -1;
  }
  return 0;
}

int goldboard::get_time(){
  return avr->cycle/16000;
}
