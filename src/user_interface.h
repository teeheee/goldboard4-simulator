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
  void update_info(Json& state);
  Json get_new_state(Json& old_state);
//private:
  std::string serial_output;
  std::string robot_info;
};

#endif
