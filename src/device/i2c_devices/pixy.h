#ifndef _PIXY_H_
#define _PIXY_H_

//TODO pixy does not work

#include "i2c_register.h"
#include <list>

struct block_t{
  int signature;
  int x;
  int y;
  int width;
  int height;
};

class pixy : public i2c_device{
public:
   std::list<struct block_t> block_list;
   int read_state;

   pixy() : i2c_device(168, "PIXY"){
     read_state = 0;
   }


   uint8_t read(){
      if(block_list.size() == 0)
         return 0;
      uint8_t data = 0;
      switch(read_state){
         case 0:
           data = 0xAA;
           break;
         case 1:
           data = 0x55;
           read_state = 0;
           break;
      }
      read_state++;
      return data;
      std::cout << "pixy read" << std::endl;
   }

   void write(uint8_t data){
     std::cout << "pixy write :" << data <<std::endl;
   }

   void set_config(Json& data){
     //TODO imptement this
   }
   void set_state(Json &data){
     Json json_blocks = data["blocks"];
     block_list.clear();
     if( json_blocks == Json::null)
        return;
    std::cout << json_blocks.stringify() << std::endl;
     for(int i = 0; i < json_blocks.size(); i++){
       Json json_block = json_blocks[i];
       struct block_t block;
       block.signature = json_block["signature"];
       block.x = json_block["x"];
       block.y = json_block["y"];
       block.width = json_block["width"];
       block.height = json_block["height"];
       block_list.push_back(block);
     }
  }

   Json get_state(){
     Json state;
     state["type"] = "PIXY";
     Json json_blocks;
     for(struct block_t block : block_list){
       Json json_block;
       json_block["signature"] = block.signature;
       json_block["x"] = block.x;
       json_block["y"] = block.y;
       json_block["width"] = block.width;
       json_block["height"] = block.height;
       json_blocks << json_block;
     }
     state["blocks"] = json_blocks;
     return state;
   }
};

#endif
