# define flags 
CC = g++ # compiler
CXX_FLAGS = -std=c++11 -I/usr/local/include -Iinclude # compiling flags
LD_FLAGS = -L/usr/local/lib -lpigpio -lpthread # linking flags

# define project executables
TARGET = main 
# define project directories
SRC_DIR = src
OBJ_DIR = obj

# get source file names given $(SRC_DIR)
SRC = $(wildcard $(SRC_DIR)/*.cpp)
# get object file names given $(OBJ_DIR)
OBJ = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC))

#All:
#	echo source_files: $(OBJ)

# default target
all: $(OBJ_DIR) $(TARGET)

# create all executable from lists
$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LD_FLAGS)

# create all objects from lists
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp 
	$(CC) -c $< -o $@ $(CXX_FLAGS)

# create object directories 
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# clean workspace
clean: 
	rm -rf $(TARGET) $(OBJ_DIR)
