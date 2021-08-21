# the compiler
CC = g++

# compiler flags:
CFLAGS = -lpthread

# the build target executable:
TARGET = spin_lock

all: $(TARGET)

	$(CC) -lpthread $(TARGET).cpp -o $(TARGET)

clean:
	$(RM) $(TARGET)