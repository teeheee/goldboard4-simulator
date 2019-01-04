#ifndef _ROBOT_H_
#define _ROBOT_H_

#include "robot.h"
#include "json11.h"

class robot{
public:
  robot(json11::json config);
  void setup();
  void run();
};

#endif
