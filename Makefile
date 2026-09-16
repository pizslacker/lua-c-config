# Compiler and linker configuration
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -O2 $(shell pkg-config --cflags lua)
LDFLAGS = 
LDLIBS = $(shell pkg-config --libs lua) -lm

# Project files
TARGET = lua_demo
SRCS = main.c
OBJS = $(SRCS:.c=.o)

# Default target built when running `make`
all: $(TARGET)

# Linking the final executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS) $(LDLIBS)

# Compiling C source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Convenience target to build and execute
run: $(TARGET)
	./$(TARGET)

# Clean up build artifacts
clean:
	rm -f $(TARGET) $(OBJS)

# Phony targets don't represent actual files on disk
.PHONY: all clean run