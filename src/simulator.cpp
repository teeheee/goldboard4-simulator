
#include <iostream>
#include <fstream>
#include <sstream>
#include "robot.h"
#include "goldboard.h"
#include "json11.h"
#include "user_interface.h"

int main(int argc, char *argv[]){
	printf("read config.json...\r\n");
	std::ifstream file_stream("config.json");
	std::stringstream buffer;
	std::string error_message = "";
	buffer << file_stream.rdbuf();
	json11::Json json_config = json11::Json::parse(buffer.str(), error_message);
	if(json_config.is_null())
	{
		printf("error parsing config.json\r\n");
		std::cout << error_message << std::endl;
	  exit(0);
	}

	printf("init robot\r\n");
	robot bot(json_config);

	printf("init user_interface\r\n");
	user_interface terminal_output;
	printf("start simulation\r\n");
	for (;;) {
		bot.run(50);
		terminal_output.serial_output += bot.gb->get_serial_data();
		terminal_output.robot_info = "";
		terminal_output.robot_info += "zeit:   \t" + std::to_string(bot.gb->get_time())  			  + " ms\n";
		terminal_output.robot_info += "motor 0: \t" + std::to_string(bot.gb->get_motor_speed(0)) + "\n";
		terminal_output.robot_info += "motor 1: \t" + std::to_string(bot.gb->get_motor_speed(1)) + "\n";
		terminal_output.robot_info += "motor 2: \t" + std::to_string(bot.gb->get_motor_speed(2)) + "\n";
		terminal_output.robot_info += "motor 3: \t" + std::to_string(bot.gb->get_motor_speed(3)) + "\n";
		terminal_output.robot_info += "led 0:   \t" + std::to_string(bot.gb->get_led_status(0)) + "\n";
		terminal_output.robot_info += "led 1:   \t" + std::to_string(bot.gb->get_led_status(1)) + "\n";
		terminal_output.robot_info += "power 0: \t" + std::to_string(bot.gb->get_power_pin(0)) + "\n";
		terminal_output.robot_info += "power 1: \t" + std::to_string(bot.gb->get_power_pin(1)) + "\n";
		terminal_output.print();
	}
	printf("finished\r\n");
}
