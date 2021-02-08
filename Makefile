CXX=c++
CXXFLAGS=-Wall
LDFLAGS=-pthread -lncurses
SRC_DIR=src
OBJ_DIR=obj
TARGET=smallchat
SRC=main.cpp client.cpp backlog.cpp
OBJ=$(SRC:%.cpp=$(OBJ_DIR)/%.o)

SERVER_SRC_DIR=server
SERVER_OBJ_DIR=obj_server
SERVER_TARGET=smallchat_server
SERVER_SRC=main.cpp server.cpp
SERVER_OBJ=$(SERVER_SRC:%.cpp=$(SERVER_OBJ_DIR)/%.o)

#client
all: setup $(TARGET)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $^ $(LDFLAGS)

#Server
server: setup $(SERVER_TARGET)

$(SERVER_OBJ_DIR)/%.o: $(SERVER_SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(SERVER_TARGET): $(SERVER_OBJ)
	$(CXX) $(CXXFLAGS) -o $(SERVER_TARGET) $^ $(LDFLAGS)

#Both
setup:
	@mkdir -p $(OBJ_DIR)/
	@mkdir -p $(SERVER_OBJ_DIR)/

clean:
	rm -r $(OBJ_DIR)/
	rm -r $(SERVER_OBJ_DIR)/

.PHONY: all clean server
