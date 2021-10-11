# the compiler
CC = g++

# compiler flags:
CFLAGS  = -pthread

TARGET_COORD = coordinator
TARGET_CLIENT = client

coordinator: $(TARGET_COORD)

$(TARGET_COORD): $(TARGET_COORD).cpp
	$(CC) $(CFLAGS) $(TARGET_COORD).cpp -o $(TARGET_COORD) ../lib/MyLib.*

client:

$(TARGET_CLIENT): $(TARGET_CLIENT).cpp
	$(CC) $(CFLAGS) $(TARGET_CLIENT).cpp -o $(TARGET_CLIENT) ../lib/MyLib.*


clean:
	$(RM) $(TARGET_COORD) $(TARGET_CLIENT)