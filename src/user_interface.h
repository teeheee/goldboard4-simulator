#ifndef _USER_INTERFACE_H_
#define _USER_INTERFACE_H_

#include "robot.h"
#include "json11.h"
#include <string>

class user_interface{
public:
  user_interface();
  ~user_interface();
  void print();
  void update_info(json11::Json *state);
//private:
  std::string serial_output;
  std::string robot_info;
};

#endif
