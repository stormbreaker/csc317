# Author: Christer Karlsson
# Written Spring 2016 for CSC317 COA.

# Usage:  make target1 target2 ...

#-----------------------------------------------------------------------

# GNU C/C++ compiler and linker:
CC = g++
# LIBS = -lpthread
FLAGS = -g -std=c++11 -Wall

# the build target executable:
TARGET = b17

all: $(TARGET)

# specific targets
b17:	b17.cpp
		$(CC) $(FLAGS) -o $@ $? $(LIBS) -lm


# utility targets
clean:
	$(RM) $(TARGET) -f *.o *~
