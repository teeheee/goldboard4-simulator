#include "user_interface.h"
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <stdlib.h>
#ifdef WIN32
#include <conio.h>
#else
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

int kbhit(void)
{
  struct termios oldt, newt;
  int ch;
  int oldf;

  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

  ch = getchar();

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);

  if(ch != EOF)
  {
    ungetc(ch, stdin);
    return 1;
  }

  return 0;
}
#endif

using namespace std;

user_interface::user_interface(){

}

user_interface::~user_interface(){

}

Json user_interface::get_new_state(Json& old_state){
  Json devices_json = old_state["devices"];
  if(kbhit()){
    int id = getc(stdin)-'0';
    if(id == 0){
        Json gb_json = old_state["goldboard"];
        cout << "choose goldboard button id:";
        int id = 0;
        cin >> id;
        Json button_json = gb_json["button"];
        button_json[id] = ((int)button_json[id]+1)%2;
        gb_json["button"] = button_json;
        old_state["goldboard"] = gb_json;
    } else if(id > 1 && id < devices_json.size()+2){
        Json device;
        int i;
        for(i = 0; i < devices_json.size(); i++){
           if((int)devices_json[i]["id"] == id)
             break;
        }
        device = devices_json[i];
        if(device["type"] == "USRING"){
          cout << "choose usring sensor to change:" << endl;
          int sensor;
          cin >> sensor;
          cout << "choose value:" << endl;
          int value;
          cin >> value;
          Json analog = device["analog"];
          analog[sensor] = value;
        }if(device["type"] == "CMPS11"){
          cout << "choose cmps11 angle:" << endl;
          int value;
          cin >> value;
          device["angle"] = value;
        }if(device["type"] == "PIXY"){
          cout << "block signature" << endl;
          int signature;
          cin >> signature;
          cout << "x:" << endl; int x; cin >> x;
          cout << "y:" << endl; int y; cin >> y;
          cout << "width:" << endl; int width; cin >> width;
          cout << "heigth:" << endl; int height; cin >> height;
          Json blocks = device["blocks"];
          Json block;
          block["signature"] = signature;
          block["x"] = x;
          block["y"] = y;
          block["width"] = width;
          block["height"] = height;
          blocks << block;
          device["blocks"] = blocks;
        }
        devices_json[i] = device;
    } else {
        cout << "wrong id" << endl;
    }
  }
  old_state["devices"] = devices_json;
  return old_state;
}

#define DISPLAY_LENGTH 15
void user_interface::print(){
  //for(int i = 0; i < DISPLAY_LENGTH*2; i++)
  //  cout << "\n";
  #ifdef WIN32
    system("cls");
  #else
    printf("\033c");
  #endif
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
        for(int i = 0; i < 60; i++)
          cout << " ";
        cout << right;
        cout << "\r";
        cout << left << "\n";
        line_count++;
  }
  for(int j = 0; j < DISPLAY_LENGTH-line_count; j++)
    cout << "\n";
}

void user_interface::update_info(Json& state){
    robot_info = "time: " + ((Json)state["time"]).stringify() + "\r\n";
    Json json_gb = state["goldboard"];
    serial_output += (string)json_gb["serial"];
    robot_info += "motors: " + ((Json)json_gb["motor"]).stringify() + "\r\n";
    robot_info += "power_pin: " + ((Json)json_gb["power_pin"]).stringify() + "\r\n";
    robot_info += "led: " + ((Json)json_gb["led"]).stringify() + "\r\n";
    robot_info += "button: " + ((Json)json_gb["button"]).stringify() + "\r\n";
    Json devices = state["devices"];
    for(int i = 0; i < devices.size(); i++){
      Json device = devices[i];
      robot_info += device.stringify() + "\r\n";
    }
}
