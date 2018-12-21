#MING_PATH = /home/ulbrich/other_stuff/mingw/bin
#CC = $(MING_PATH)/x86_64-w64-mingw32-gcc
#AS = $(MING_PATH)/x86_64-w64-mingw32-as
#CXX = $(MING_PATH)/x86_64-w64-mingw32-c++

TARGET_EXEC ?= simulator

BUILD_DIR ?= ./build
SRC_DIRS ?= ./src

#INC_DIRS = simavr simavr/cores simavr/cores/avr simavr/sim simavr/sim/avr

SRCS := $(shell find $(SRC_DIRS) -name "*.cpp" -or -name "*.c" -or -name "*.s")
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS ?= $(INC_FLAGS) -MMD -MP -lutil -lrt #--std=gnu99 -I./avr-libc/include 
LDFLAGS ?= -MMD -MP -lutil -lrt 

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS); cp $(BUILD_DIR)/$(TARGET_EXEC) $(TARGET_EXEC)

# assembly
$(BUILD_DIR)/%.s.o: %.s
	$(MKDIR_P) $(dir $@)
	$(AS) $(ASFLAGS) -c $< -o $@

# c source
$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# c++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@


.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)

-include $(DEPS)

MKDIR_P ?= mkdir -p
