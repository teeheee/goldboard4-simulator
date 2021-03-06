

TARGET_EXEC ?= simulator

ifeq ($(OS),windows)
  MING_PATH = C:/Mingw/bin/
  CC = $(MING_PATH)x86_64-w64-mingw32-gcc
  AS = $(MING_PATH)x86_64-w64-mingw32-as
  CXX = $(MING_PATH)x86_64-w64-mingw32-c++
  TARGET_EXEC = simulator.exe
else ifeq ($(OS),windows_cross)
  CC = i686-w64-mingw32-gcc
  AS = i686-w64-mingw32-as
  CXX = i686-w64-mingw32-g++
  TARGET_EXEC = simulator.exe
else ifeq ($(OS),linux)
  CC = gcc
  AS = as
  CXX = g++
  TARGET_EXEC = simulator
else
  $(info OS not set! can be [windows, windows_cross, linux])
  exit 0
endif


BUILD_DIR ?= ./build
SRC_DIRS ?= ./src

#INC_DIRS = simavr simavr/cores simavr/cores/avr simavr/sim simavr/sim/avr

SRCS := $(shell find $(SRC_DIRS) -name "*.cpp" -or -name "*.c" -or -name "*.s")
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CFLAGS ?= $(INC_FLAGS) -MMD -MP  --std=gnu99 -DMCU=atmega32
CXXFLAGS ?= $(INC_FLAGS) -MMD -MP -DMCU=atmega32 -std=gnu++11
LDFLAGS ?= -MMD -MP


ifeq ($(OS),windows)
  CFLAGS += -lws2_32 -I./avr-libc/include -static-libstdc++
  CXXFLAGS += -lws2_32 -I./avr-libc/include -static-libstdc++
  LDFLAGS += -lws2_32 -static-libstdc++
endif

ifeq ($(OS),windows_cross)
  CFLAGS += -lpthread -lws2_32
  CXXFLAGS += -lpthread -lws2_32
  LDFLAGS += -lpthread -lws2_32
endif

ifeq ($(OS),linux)
  CFLAGS += -lpthread -lrt -lutil -lncurses
  LDFLAGS += -lpthread -lrt -lutil -lncurses
  CXXFLAGS += -lpthread -lrt -lutil -lncurses
endif

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS); cp $(BUILD_DIR)/$(TARGET_EXEC) $(TARGET_EXEC)

# c source
$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# cpp source
$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)

install:
	cp simulator /usr/bin/simulator

-include $(DEPS)

MKDIR_P ?= mkdir -p
