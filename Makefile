APP_NAME := simpleapp
SRC_DIR := src
TEST_DIR := tests
OBJ_DIR := obj
BIN_DIR := exe

CXX := c++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic -I$(SRC_DIR)
DEPFLAGS := -MMD -MP
LDFLAGS := -lncurses -pthread

SOURCES := $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(TEST_DIR)/*.cpp)
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(filter $(SRC_DIR)/%.cpp,$(SOURCES))) \
		   $(patsubst $(TEST_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(filter $(TEST_DIR)/%.cpp,$(SOURCES)))
DEPS := $(OBJECTS:.o=.d)
TARGET := $(BIN_DIR)/$(APP_NAME)

.PHONY: all clean

all: $(TARGET)

-include $(DEPS)

$(TARGET): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(DEPS) $(TARGET)
