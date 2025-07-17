# Compiler and Flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11

# Source files
SRC = main.c vm_manager.c
OBJ = $(SRC:.c=.o)

# Output binary name
TARGET = vm_manager

# Default target
all: $(TARGET)

# Linking object files into final binary
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Clean build artifacts
clean:
	del /Q *.o *.exe 2>nul || rm -f *.o $(TARGET)

# For WSL/UNIX sys
.PHONY: all clean