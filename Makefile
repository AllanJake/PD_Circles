SDK_PATH ?= $(shell playdate-config sdk-path)
CXX = arm-none-eabi-g++
CXXFLAGS = -I$(SDK_PATH)/C_API -I./src/Header -std=c++17 -Os -g
LDFLAGS = -Wl,-T,$(SDK_PATH)/C_API/buildsupport/link_map.LDFLAGS

SRC_DIR = src/Source
BUILD_DIR = build

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

TARGET = $(BUILD_DIR)/CirclesInC.pdx

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

all: $(TARGET)

%(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) -o $(BUILD_DIR)/pdex.elf
	$(SDK_PATH)/bin/pdc $(BUILD_DIR) $(TARGET)

clean:
	rm -rf $(BUILD_DIR)