# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -O2

# Source files
SRCS = assembler.c parser.c export.c symbolTable.c

# Object files
OBJS = $(SRCS:.c=.o)

# Executable name
TARGET = assembler

# Default target
all: $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

# Compile C source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean the generated files
clean:
	rm -f $(OBJS) $(TARGET)
