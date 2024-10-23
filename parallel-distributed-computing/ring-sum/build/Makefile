# Define the compiler (MPI compiler)
CC = mpicc

# Compiler flags
CFLAGS = 

# Source files
SOURCES = ../main.c

# Target executable
TARGET = ring-sum.out

# Default target to build the executable
all: $(TARGET)

# Rule to build the target
$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

# Clean up generated files
.PHONY: all clean
clean:
	rm -f $(TARGET)
