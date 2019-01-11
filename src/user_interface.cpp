#include "user_interface.h"
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

user_interface::user_interface(){

}

user_interface::~user_interface(){

}

#define DISPLAY_LENGTH 15

void user_interface::print(){
  for(int i = 0; i < 100; i++)
    cout << "\n";

  size_t cut = serial_output.length();
  size_t length = serial_output.length();
  size_t cut2 = 0;
  for(int i = 0;; i++)
  {
    size_t tmp = serial_output.rfind('\n', cut-1);
    if(tmp == string::npos)
      break;
    cut = tmp;
    if( i >= DISPLAY_LENGTH){
      serial_output = serial_output.substr(cut, length-cut);
      cut2 = length-cut;
      break;
    }
  }

  stringstream serial(serial_output + "\n");
  stringstream info(robot_info  + "\n");
  string left, right;
  int line_count = 0;
  while(1){
        int exit_flag = 0;
        if(!getline(serial, left, '\n'))
          exit_flag ++;
        if(!getline(info, right, '\n'))
          exit_flag ++;

        if(exit_flag == 2)
          break;
        left.erase(remove(left.begin(), left.end(), '\n'), left.end());
        right.erase(remove(right.begin(), right.end(), '\n'), right.end());
        left.erase(remove(left.begin(), left.end(), '\r'), left.end());
        right.erase(remove(right.begin(), right.end(), '\r'), right.end());

        cout << "\r";
        for(int i = 0; i < 80; i++)
          cout << " ";
        cout << right;
        cout << "\r";
        cout << left << "\n";
        line_count++;
  }
  for(int j = 0; j < DISPLAY_LENGTH-line_count; j++)
    cout << "\n";
}

void user_interface::update_info(json11::Json *state){

}
