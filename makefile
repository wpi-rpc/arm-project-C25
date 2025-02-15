# define flags 
CC = g++ # compiler
CXX_FLAGS = -std=c++11 -I/usr/local/include # compiling flags
LD_FLAGS = -L/usr/local/lib -lpigpio -lpthread # linking flags

# define files
TARGET = main # produced executables
SRC = main.cpp # source files
OBJ = main.o # object files

# default target
all: $(TARGET)

# create all executable from lists
$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $(OBJ) $(LD_FLAGS)

# create all objects from lists
$(OBJ): $(SRC)
	$(CC) -c $(SRC) $(CXX_FLAGS)

# clean workspace
clean: 
	rm -f $(TARGET) $(OBJ)
