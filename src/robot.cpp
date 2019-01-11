#include "robot.h"
#include <iostream>

robot::robot(json11::Json &config){
  if(!config["firmware"].is_string()){
    printf("firmware string missing in config.json\r\n");
    exit(0);
  }

  atmega = new atmega32(config["firmware"].string_value().c_str());
  gb = new goldboard(atmega->get_simavr_avr());

  if(!config["i2c_devices"].is_array()){
    printf("i2c_devices is not an array\r\n");
    exit(0);
  }
  std::vector<json11::Json> json_i2c_devices = config["i2c_devices"].array_items();

  for(int i = 0; i < json_i2c_devices.size(); i++){
    if(!json_i2c_devices[i]["type"].is_string()){
        printf("i2c_device attribute type is not an string\r\n");
        exit(0);
    }

    if(json_i2c_devices[i]["type"].string_value() == std::string("CMPS11")){
      printf("added CMPS11\r\n");
      add_i2c_device(new cmps11());
    } else if(json_i2c_devices[i]["type"].string_value() == std::string("PIXY")){
      printf("added PIXY\r\n");
      add_i2c_device(new pixy());
    } else if(json_i2c_devices[i]["type"].string_value() == std::string("PCF8574")){
      printf("added PCF8574\r\n");
      add_i2c_device(new pcf8574(json_i2c_devices[i]["address"].int_value()));
    } else if(json_i2c_devices[i]["type"].string_value() == std::string("USRING")){
      printf("added USRING\r\n");
      add_i2c_device(new usring());
    } else {
      std::cout << "unknown i2c device: " << json_i2c_devices[i]["type"].string_value() << std::endl;
      exit(0);
    }
  }
}

robot::~robot(){
  delete gb;
  delete atmega;
  for (i2c_device* device : i2c_device_list) {
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


json11::Json* robot::get_state(){
  
}


void robot::add_i2c_device(i2c_device* device){
  i2c_device_list.push_front(device);
  gb->add_i2c_device(*device);
}
