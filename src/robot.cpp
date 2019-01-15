#include "robot.h"
#include <iostream>

robot::robot(Json &config){
  if(config["firmware"] == Json::null){
    printf("firmware string missing in config.json\r\n");
    exit(0);
  }
  std::string file_name = config["firmware"];
  atmega = new atmega32(file_name.c_str());
  gb = new goldboard(atmega->get_simavr_avr());

  printf("start parsing devices\r\n");
  Json device_config = config["devices"];
  int number_of_devices = device_config.size();

  for(int i = 0; i < number_of_devices; i++){
    std::string type = config["devices"][i]["type"];
    std::cout << "generate device " << type << std::endl;

    if(type == std::string("CMPS11")){
      printf("added CMPS11\r\n");
      i2c_device* d = new cmps11();
      add_device(d);
    } else if(type == std::string("PIXY")){
      printf("added PIXY\r\n");
      add_device(new pixy());
    } else if(type == std::string("PCF8574")){
      printf("added PCF8574\r\n");
      int address = device_config[i]["address"];
      add_device(new pcf8574(address));
    } else if(type == std::string("USRING")){
      printf("added USRING\r\n");
      add_device(new usring());
    } else {
      std::cout << "unknown device: " << type << std::endl;
      exit(0);
    }
  }
}

robot::~robot(){
  delete gb;
  delete atmega;
  for (device* device : device_list) {
        delete device;
  }
}

void robot::run(int ms){
  if(gb->run(ms))
  {
    printf("avr simulation error\r\n");
    exit(0);
  }
}


Json robot::get_state(){
  Json data = gb->get_state();
  Json json_devices = Json::array();
  int index = 0;
  for(device* part : device_list){
    json_devices[index++] = part->get_state();
  }
  data["devices"] = json_devices;
  return data;
}


void robot::set_state(Json& data){
  gb->set_state(data);
  Json json_devices = data["devices"];
  for(int json_index = 0; json_index < json_devices.size(); json_index++){
    for(device* part : device_list){
      int json_id = json_devices[json_index]["id"];
      int device_id = part->get_id();
      if(json_id == device_id){
        Json part_state = json_devices[json_index];
        part->set_state(part_state);
      }
    }
  }
}

void robot::add_device(device* device){
  device_list.push_front(device);
  if(device->get_type() == I2C_TYPE){
    gb->add_i2c_device((i2c_device&)*device);
  }else if(device->get_type() == DIGITAL_TYPE){
    printf("type not implemented");
  }
}
