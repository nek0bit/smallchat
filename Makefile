CXX=c++
CXXFLAGS=-Wall
LDFLAGS=-pthread -lncurses
SRC_DIR=src
OBJ_DIR=obj
TARGET=smallchat
SRC=main.cpp client.cpp
OBJ=$(SRC:%.cpp=$(OBJ_DIR)/%.o)

all: setup $(TARGET)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $^ $(LDFLAGS)

setup:
	@mkdir -p $(OBJ_DIR)/

clean:
	rm -r $(OBJ_DIR)/

.PHONY: all clean
