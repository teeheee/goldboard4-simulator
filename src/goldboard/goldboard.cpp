#include "goldboard.h"

goldboard::goldboard(struct avr_t* aavr)
        : pcf_motor(0x70),
          pcf_digital(0x7E),
          uart_obj(aavr)
{
  printf("init goldboard\r\n");
  avr = aavr;
  button_state[0] = 0;
  button_state[1] = 0;
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
  Json button = data["button"];
  set_button_status(0, (int)button[0]);
  set_button_status(1, (int)button[1]);
}

Json goldboard::get_state(){
  Json json_data;

  json_data["time"] = get_time();

  Json json_motor = Json {
    get_motor_speed(0),
    get_motor_speed(1),
    get_motor_speed(2),
    get_motor_speed(3)
  };

  Json json_led = Json {
    get_led_status(0),
    get_led_status(1)
  };

  Json json_power_pin = Json {
    get_power_pin(0),
    get_power_pin(1)
  };


  Json json_button = Json {
    button_state[0],
    button_state[1]
  };

  Json json_serial = Json(get_serial_data().c_str());

  Json json_goldboard;
  json_goldboard["motor"] = json_motor;
  json_goldboard["power_pin"] = json_power_pin;
  json_goldboard["led"] = json_led;
  json_goldboard["button"] = json_button;
  json_goldboard["serial"] = json_serial;
  json_data["goldboard"] = json_goldboard;

  return json_data;
}



//interface function
int goldboard::get_led_status(int id){
  return 0;
}

double goldboard::get_motor_speed(int id){
  if(pcf_motor.read() & (1 << id))
    return get_pwm_value(&motor_pwm[id]);
  else if(pcf_motor.read() & (1 << id+1))
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
  button_state[id] = state;
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
