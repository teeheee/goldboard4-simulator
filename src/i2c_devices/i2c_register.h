#ifndef _I2C_REGISTER_H_
#define _I2C_REGISTER_H_

#include "i2c_device.h"

template<unsigned int MEM_SIZE> class i2c_register : public i2c_device{
public:
   i2c_register(uint8_t aaddress, const char* aname)
   : i2c_device(aaddress, aname){ };
   uint8_t read(){
     int tmp_index = register_index;
     register_index = (register_index+1)%MEM_SIZE;
     return output_data[tmp_index];
   };
   void write(uint8_t data){
     if( data < MEM_SIZE )
        register_index = MEM_SIZE;
   };
   void set_byte(unsigned int index, uint8_t data){
     if( index < MEM_SIZE )
        output_data[index] = data;
   };
private:
  uint8_t output_data[MEM_SIZE] = {0};
  int register_index = 0;
};

#endif
