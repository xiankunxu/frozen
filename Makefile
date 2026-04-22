# Compiler and compiler flags
CC := gcc
CFLAGS := -Iinclude -g # -DJSON_ENABLE_BASE64=1 JSON_ENABLE_BASE64 is by default enabled in json.h, so we don't need to define it here unless we want to disable it. 

# Source files and output executable
SRCDIR := .
SOURCES := $(wildcard $(SRCDIR)/*.c)
OBJECTS := $(SOURCES:$(SRCDIR)/%.c=%.o)
EXECUTABLE := my_program

# Targets
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(EXECUTABLE) $(OBJECTS)

.PHONY: all clean

