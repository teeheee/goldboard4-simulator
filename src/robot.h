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
  robot(Json &config);
  ~robot();
  void run(int ms);
  Json get_state();
  void set_state(Json& data);

  goldboard* gb; //should be private
private:
  void add_device(device* device);

  atmega32* atmega;
  std::list<device*> device_list;
};

#endif
