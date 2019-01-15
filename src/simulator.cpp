
#include <iostream>
#include <fstream>
#include <sstream>
#include "robot.h"
#include "goldboard.h"
#include "json11.h"
#include "user_interface.h"

int main(int argc, char *argv[]){
	printf("read config.json\r\n");
	std::ifstream file_stream("config.json");
	std::stringstream buffer;
	std::string error_message = "";
	buffer << file_stream.rdbuf();
	Json json_config = Json::parse(buffer.str());
	if(json_config == Json::null)
	{
		printf("error parsing config.json\r\n");
	  exit(0);
	}

	printf("init robot\r\n");
	robot bot(json_config);

	printf("init user_interface\r\n");
	user_interface terminal_output;
	printf("start simulation\r\n");
	for (;;) {
		bot.run(50);
		terminal_output.serial_output = "";
		terminal_output.robot_info = "";
		//terminal_output.print();
	}
	printf("finished\r\n");
}
