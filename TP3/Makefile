# the compiler
CC = g++

# compiler flags:
CFLAGS  = -pthread

TARGET_COORD = coordinator
TARGET_CLIENT = client

all: $(TARGET_COORD)

$(TARGET_COORD): $(TARGET_COORD).cpp
	$(CC) $(CFLAGS) $(TARGET_COORD).cpp -o $(TARGET_COORD)

$(TARGET_CLIENT): $(TARGET_CLIENT).cpp
	$(CC) $(CFLAGS) $(TARGET_CLIENT).cpp -o $(TARGET_CLIENT)

clean:
	$(RM) $(TARGET_COORD) $(TARGET_CLIENT)