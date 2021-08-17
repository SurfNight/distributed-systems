# the compiler
CC = g++

# compiler flags:
CFLAGS  = -Wall

# the build target executable:
TARGET = producer_consumer

all: $(TARGET)

	$(TARGET): $(TARGET).cpp
	$(CC) $(CFLAGS) -o $(TARGET) ../../lib/MyLib.cpp ../../lib/MyLib.h $(TARGET).cpp

clean:
	$(RM) $(TARGET)