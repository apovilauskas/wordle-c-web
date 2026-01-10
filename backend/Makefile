CC = gcc
CFLAGS = -Wall -Wextra -std=c11
LDFLAGS = -lws2_32 

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