#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <libgen.h>
#include <string.h>
#include <signal.h>
#include "sim_avr.h"
#include "sim_elf.h"
#include "sim_core.h"
#include "sim_gdb.h"
#include "sim_hex.h"
#include "sim_vcd_file.h"

#include "sim_core_decl.h"
#include "goldboard.h"

#include "cmps11.h"
#include "pixy.h"
#include "usring.h"

#include "server.h"
#include "json11.h"

static avr_t * avr = NULL;

void setup_avr();

int main(int argc, char *argv[]){
	setup_avr();

	printf("init goldboard\r\n");
	goldboard gb(avr);

	printf("init peripherie\r\n");
	cmps11 kompass(avr);
	pixy video(avr);
	usring ring(avr);
	pcf8574 pcf1(avr, 0x4A);
	pcf8574 pcf2(avr, 0x44);
	pcf8574 pcf3(avr, 0x45);

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


void setup_avr(){
	const char* firmware_file_name = "gb_programm.hex";

	elf_firmware_t f = {{0}};
	uint32_t loadBase = AVR_SEGMENT_OFFSET_FLASH;
	ihex_chunk_p chunk = NULL;
	int cnt = read_ihex_chunks(firmware_file_name, &chunk);
	if (cnt <= 0) {
		fprintf(stderr, "Unable to load IHEX file %s\n", firmware_file_name);
		exit(1);
	}
	printf("Loaded %d section of ihex\n", cnt);
	for (int ci = 0; ci < cnt; ci++) {
		if (chunk[ci].baseaddr < (1*1024*1024)) {
			f.flash = chunk[ci].data;
			f.flashsize = chunk[ci].size;
			f.flashbase = chunk[ci].baseaddr;
			printf("Load HEX flash %08x, %d\n", f.flashbase, f.flashsize);
		} else if (chunk[ci].baseaddr >= AVR_SEGMENT_OFFSET_EEPROM ||
				chunk[ci].baseaddr + loadBase >= AVR_SEGMENT_OFFSET_EEPROM) {
			// eeprom!
			f.eeprom = chunk[ci].data;
			f.eesize = chunk[ci].size;
			printf("Load HEX eeprom %08x, %d\n", chunk[ci].baseaddr, f.eesize);
		}
	}
	strcpy(f.mmcu, "atmega32\0");
	f.frequency = 16000000;
	avr = avr_make_mcu_by_name(f.mmcu);
	avr_init(avr);
	avr->log = 0;
	avr_load_firmware(avr, &f);
	avr->pc = f.flashbase;
	avr->gdb_port = 1234;
}
