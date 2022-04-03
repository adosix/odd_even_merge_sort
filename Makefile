# the compiler: gcc for C program, define as g++ for C++
CC = mpic++

# compiler flags:
#  -g     - this flag adds debugging information to the executable file
#  -Wall  - this flag is used to turn on most compiler warnings
CFLAGS  = -g -Wall -fopenmp -std=c++11

# The build target 
TARGET = oems

all: $(TARGET)

$(TARGET): $(TARGET).cpp
			$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).cpp

clean:
			$(RM) $(TARGET)