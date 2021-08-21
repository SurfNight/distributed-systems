# the compiler
CC = g++

# compiler flags:
CFLAGS  = -lpthread

# the build target executable:
TARGET = spin_lock

all: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CC) $(CFLAGS) $(TARGET).cpp -o $(TARGET)

clean:
	$(RM) $(TARGET)