#ifndef _ROBOT_H_
#define _ROBOT_H_

#include "goldboard.h"
#include "atmega32.h"
#include "json11.h"
#include "cmps11.h"
#include "pixy.h"
#include "usring.h"
#include "server.h"
#include "json11.h"
#include "i2c_device.h"
#include <list>


class robot{
public:
  robot(json11::Json &config);
  ~robot();
  void run(int ms);
  json11::Json* get_state();

  goldboard* gb; //should be private
private:
  void add_i2c_device(i2c_device* device);

  atmega32* atmega;
  std::list<i2c_device*> i2c_device_list;
};

#endif
