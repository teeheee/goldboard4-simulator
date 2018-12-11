#
# 	Copyright 2008-2012 Michel Pollet <buserror@gmail.com>
#
#	This file is part of simavr.
#
#	simavr is free software: you can redistribute it and/or modify
#	it under the terms of the GNU General Public License as published by
#	the Free Software Foundation, either version 3 of the License, or
#	(at your option) any later version.
#
#	simavr is distributed in the hope that it will be useful,
#	but WITHOUT ANY WARRANTY; without even the implied warranty of
#	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#	GNU General Public License for more details.
#
#	You should have received a copy of the GNU General Public License
#	along with simavr.  If not, see <http://www.gnu.org/licenses/>.

target=	goldboard_test
firm_src = ${wildcard at*${board}.c}
#firmware = ${firm_src:.c=.axf}
simavr = ..

IPATH = .
IPATH += ${simavr}/examples/shared
IPATH += ${simavr}/examples/parts
IPATH += ${simavr}/include
IPATH += ${simavr}/simavr/sim

VPATH = .
#VPATH += ${simavr}/examples/shared
#VPATH += ${simavr}/examples/parts


all: obj ${target}

include ${simavr}/Makefile.common

board = ${OBJ}/${target}.elf

${board} : ${OBJ}/i2c_sensor.o
${board} : ${OBJ}/i2c_device.o
${board} : ${OBJ}/cmps11.o
${board} : ${OBJ}/pcf8574.o
${board} : ${OBJ}/motor.o
${board} : ${OBJ}/${target}.o
#${board} : ${OBJ}/i2c_eeprom.o

${target}: ${board}
	@echo $@ done
	cp ${OBJ}/${target}.elf test

clean: clean-${OBJ}
	rm -rf *.hex *.a *.axf ${target} *.vcd .*.swo .*.swp .*.swm .*.swn
