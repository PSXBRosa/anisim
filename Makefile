CXX := g++
CXXFLAGS := -Wall -g -std=c++20
LDFLAGS :=

SRC_DIR := src
INCLUDE_DIR := include
TEST_DIR := tests
OBJ_DIR := build

SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
TEST_FILES := $(wildcard $(TEST_DIR)/*.cpp)

OBJ_FILES := $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(SRC_FILES))
TEST_OBJ_FILES := $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(TEST_FILES))

TARGET := main
TEST_TARGET := test

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

$(OBJ_DIR)/%.o: $(TEST_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

$(TARGET): $(OBJ_DIR) $(OBJ_FILES)
	$(CXX) $(LDFLAGS) -o $@ $(OBJ_DIR)/main.o $(OBJ_FILES)

$(TEST_TARGET): $(OBJ_DIR) $(TEST_OBJ_FILES)
	$(CXX) $(LDFLAGS) -o $@ $(TEST_OBJ_FILES)

all: $(TARGET)

test: $(TEST_TARGET)

clean:
	rm -rf $(OBJ_DIR) $(TARGET) $(TEST_TARGET)

