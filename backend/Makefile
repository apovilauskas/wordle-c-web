CC = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGET = wordle_server
# Use wildcard to make sure it catches ALL .c files in the folder
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o $(TARGET)