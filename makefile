#------------------------------------------------------------------------------
# IO Commands
#------------------------------------------------------------------------------

NOCOLOR=\033[0m
BLACK=\033[0;30m
RED=\033[0;31m
GREEN=\033[0;32m
ORANGE=\033[0;33m
BLUE=\033[0;34m
PURPLE=\033[0;35m
CYAN=\033[0;36m
LIGHTGRAY=\033[0;37m
DARKGRAY=\033[1;30m
LIGHTRED=\033[1;31m
LIGHTGREEN=\033[1;32m
YELLOW=\033[1;33m
LIGHTBLUE=\033[1;34m
LIGHTPURPLE=\033[1;35m
LIGHTCYAN=\033[1;36m
WHITE=\033[1;37m

#------------------------------------------------------------------------------
# Directories
#------------------------------------------------------------------------------

MKL_DIR = /opt/intel/compilers_and_libraries/linux/mkl
MKL_INC_DIR = $(MKL_DIR)/include
MKL_LIB_DIR = $(MKL_DIR)/lib/intel64

#------------------------------------------------------------------------------
# Compiler Commands
#------------------------------------------------------------------------------

CC = g++-5
GNU_FLAGS = -O4 -Wall -g -Wextra -std=c++1y -m64 -mtune=corei7-avx -pthread
MKL_FLAGS = -DMKL_ILP64 -I$(MKL_INC_DIR)

CFLAGS= $(GNU_FLAGS) $(MKL_FLAGS)

#------------------------------------------------------------------------------
# Linker Commands
#------------------------------------------------------------------------------

ARMA_LIB = -larmadillo
LIBS= $(ARMA_LIB)

#------------------------------------------------------------------------------
# Compilation
#------------------------------------------------------------------------------

HPP_FILES := $(wildcard src/*.hpp src/ai/*.hpp src/fn/*.hpp src/io/*.hpp)

CPP_FILES = $(wildcard src/*.cpp)

OBJ_FILES := $(patsubst src/%.cpp, obj/%.o, $(CPP_FILES))

TARGET = application

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	@echo "----------------------------------------------------------------------"
	@echo "${LIGHTCYAN}Linking${NOCOLOR} $@"
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
	@echo "----------------------------------------------------------------------"
	@echo " "

$(OBJ_FILES): $(CPP_FILES)
	@echo "----------------------------------------------------------------------"
	@echo "${LIGHTCYAN}Compiling${NOCOLOR} $<"
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "----------------------------------------------------------------------"
	@echo " "

clean:
	rm -rf obj/*.o application

indent:
	astyle --style=allman --indent-switches src/*.cpp src/*.hpp
