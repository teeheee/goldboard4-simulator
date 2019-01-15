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
   void set_state(Json &data){
      printf("Warning! This set data is not impemented\r\n");
   }
   Json get_state(){
        printf("Warning! This get data is not impemented\r\n");
        return Json();
   }
   int get_id(){
     return id;
   }
   int get_type(){
     return NO_TYPE;
   }
   static int id_counter;
};

#endif
