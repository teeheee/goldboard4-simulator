
#include <iostream>

#include "atmega32.h"
#include "goldboard.h"
#include "cmps11.h"
#include "pixy.h"
#include "usring.h"
#include "server.h"
#include "json11.h"
#include "i2c_device.h"


int main(int argc, char *argv[]){
	atmega32 atmega("gb_programm.hex");

	avr_t * avr = atmega.get_simavr_avr();

	printf("init goldboard\r\n");
	goldboard gb(avr);

	printf("init peripherie\r\n");
	cmps11 kompass;
	pixy video;
	usring ring;
	pcf8574 pcf1(0x4A);
	pcf8574 pcf2(0x44);
	pcf8574 pcf3(0x45);

	gb.add_i2c_device(kompass);
	gb.add_i2c_device(video);
	gb.add_i2c_device(ring);
	gb.add_i2c_device(pcf1);
	gb.add_i2c_device(pcf2);
	gb.add_i2c_device(pcf3);

	printf("start simulation\r\n");
	for (;;) {
		int state = avr_run(avr);
		if (state == cpu_Done || state == cpu_Crashed)
			break;
		std::cout << gb.get_serial_data();
	}
	printf("finished\r\n");
	avr_terminate(avr);
}
