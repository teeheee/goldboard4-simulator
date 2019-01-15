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

    i2c_device* d = NULL;
    if(type == std::string("CMPS11"))
      d = new cmps11();
    else if(type == std::string("PIXY"))
      d = new pixy();
    else if(type == std::string("PCF8574"))
      d = new pcf8574();
    else if(type == std::string("USRING"))
      d = new usring();

    if(d != NULL){
      Json config = device_config[i];
      d->set_config(config);
      printf("added %s\r\n", type.c_str());
      add_device(d);
    } else {
      printf("unknown device\r\n");
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
  for(device* part : device_list){
    Json state = part->get_state();
    state["id"] = part->get_id();
    json_devices << state;
  }
  data["devices"] = json_devices;
  return data;
}


void robot::set_state(Json& data){
  Json json_goldboard = data["goldboard"];
  gb->set_state(json_goldboard);
  Json json_devices = data["devices"];
  for(int json_index = 0; json_index < json_devices.size(); json_index++){
    for(device* part : device_list){
      Json json_part = json_devices[json_index];
      int json_id = json_part["id"];
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
