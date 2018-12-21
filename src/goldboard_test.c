/*
	i2ctest.c

	Copyright 2008-2011 Michel Pollet <buserror@gmail.com>

 	This file is part of simavr.

	simavr is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	simavr is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with simavr.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include <pthread.h>

#include "sim_avr.h"
#include "sim_elf.h"
#include "sim_gdb.h"
#include "sim_vcd_file.h"
#include "sim_time.h"
#include "avr_twi.h"
#include "uart.h"
#include "sim_hex.h"

#include "motor.h"
#include "pcf8574.h"
#include "cmps11.h"


avr_t * avr = NULL;
avr_vcd_t vcd_file;


int main(int argc, char *argv[])
{
	elf_firmware_t f;
	const char * fname =  "gb_programm.hex";
	ihex_chunk_p chunk = NULL;
	memcpy(f.mmcu,"atmega32\0",9);
	f.frequency = 16000000UL;
  uint32_t loadBase = AVR_SEGMENT_OFFSET_FLASH;
	int cnt = read_ihex_chunks(fname, &chunk);
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
	avr = avr_make_mcu_by_name(f.mmcu);
	avr_init(avr);
	avr_load_firmware(avr, &f);
	avr->gdb_port = 1234;
	if (0) {
			avr_gdb_init(avr);
	}


	pcf8574_t pcf1;
	pcf8574_t pcf2;
	cmps11_t cmps;
	uart_t uart;

	pcf8574_init(avr,&pcf1,0x70);
	pcf8574_init(avr,&pcf2,0x74);
	init_motor(avr, &pcf1);
	cmps11_init(avr,&cmps,0xC0);
	uart_init(avr, &uart);


	printf( "Simulation launching:\n");
	int state = cpu_Running;
	while ((state != cpu_Done) && (state != cpu_Crashed))
	{
		state = avr_run(avr);
		if(avr->cycle%100==0)
		{
      printf("running\r\n");
		}
	}
}
