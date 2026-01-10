CC = gcc
CFLAGS = -Wall -Wextra -std=c11
# Removed -lws2_32 because it is for Windows only
LDFLAGS = 

TARGET = wordle_server
SOURCES = server.c http_handler.c game_state.c logic.c words.c
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run