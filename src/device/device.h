#ifndef _DEVICE_H_
#define _DEVICE_H_

#include "json11.h"

#define NO_TYPE 0
#define I2C_TYPE 1
#define DIGITAL_TYPE 2
#define ANALOG_TYPE 3

class device{
private:
  int id;
public:
   device(){
      printf("init device\r\n");
      id = id_counter;
      id_counter++;
   }
   virtual void set_state(Json &data) = 0;
   virtual Json get_state() = 0;
   virtual void set_config(Json &data) = 0;
   virtual int get_type() = 0;
   int get_id(){
     return id;
   }
   static int id_counter;
};

#endif
