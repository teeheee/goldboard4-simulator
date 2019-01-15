
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
#include "atmega32.h"

atmega32::atmega32(const char* firmware_file_name){

	elf_firmware_t f = {{0}};
	uint32_t loadBase = AVR_SEGMENT_OFFSET_FLASH;
	ihex_chunk_p chunk = NULL;
	int cnt = read_ihex_chunks(firmware_file_name, &chunk);
	if (cnt <= 0) {
		fprintf(stderr, "Unable to load IHEX file %s\n", firmware_file_name);
		exit(1);
	}
	//printf("Loaded %d section of ihex\n", cnt);
	printf("load firmware\r\n");
	for (int ci = 0; ci < cnt; ci++) {
		if (chunk[ci].baseaddr < (1*1024*1024)) {
			f.flash = chunk[ci].data;
			f.flashsize = chunk[ci].size;
			f.flashbase = chunk[ci].baseaddr;
			printf("\tLoad HEX flash %08x, %d\n", f.flashbase, f.flashsize);
		} else if (chunk[ci].baseaddr >= AVR_SEGMENT_OFFSET_EEPROM ||
				chunk[ci].baseaddr + loadBase >= AVR_SEGMENT_OFFSET_EEPROM) {
			// eeprom!
			f.eeprom = chunk[ci].data;
			f.eesize = chunk[ci].size;
			printf("\tLoad HEX eeprom %08x, %d\n", chunk[ci].baseaddr, f.eesize);
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

struct avr_t* atmega32::get_simavr_avr(){
  return avr;
}
