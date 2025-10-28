# Variables
CC = gcc
CFLAGS = -Wall -Werror -std=c99
TARGET = codec
SOURCES = codec.c compress.c
OBJECTS = $(SOURCES:.c=.o)
HEADERS = compress.h

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)
