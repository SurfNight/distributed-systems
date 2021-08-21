# the compiler
CC = g++

# compiler flags:
CFLAGS  = -lpthread

# the build target executable:
TARGET = producer_consumer

all: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CC) $(TARGET).cpp ../../lib/MyLib.* -o $(TARGET) $(CFLAGS)

clean:
	$(RM) $(TARGET)