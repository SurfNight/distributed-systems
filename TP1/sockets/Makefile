# the compiler
CC = g++

# compiler flags:
CFLAGS  = -Wall

# the build target executable:
TARGET_ONE = consumer
TARGET_TWO = producer

all: $(TARGET_ONE) $(TARGET_TWO)

$(TARGET_ONE): $(TARGET_ONE).cpp
	$(CC) $(CFLAGS) -o $(TARGET_ONE) ../lib/MyLib.cpp ../lib/MyLib.h $(TARGET_ONE).cpp

$(TARGET_TWO): $(TARGET_TWO).cpp
	$(CC) $(CFLAGS) -o $(TARGET_TWO) $(TARGET_TWO).cpp

clean:
	$(RM) $(TARGET_ONE) $(TARGET_TWO)