APP_NAME := simpleapp
SRC_DIR := src
TEST_DIR := tests
OBJ_DIR := obj
BIN_DIR := exe

CXX := c++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic -I$(SRC_DIR)
LDFLAGS := -lncurses -pthread

SOURCES := $(SRC_DIR)/App.cpp $(SRC_DIR)/Window.cpp $(SRC_DIR)/Label.cpp $(TEST_DIR)/simpleapp.cpp
OBJECTS := $(OBJ_DIR)/App.o $(OBJ_DIR)/Window.o $(OBJ_DIR)/Label.o $(OBJ_DIR)/simpleapp.o
TARGET := $(BIN_DIR)/$(APP_NAME)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)
