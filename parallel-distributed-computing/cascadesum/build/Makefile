CC = mpicc
SOURCES = ../src/cascadesum/cascadesum.c \
          ../../common/integer/integer.c \
          ../../common/vector/vector.c \
          ../src/main.c
TARGET = cascadesum.out

# Default target
all: $(TARGET)

# Rule to build the target
$(TARGET): $(SOURCES)
	$(CC) $(SOURCES) -o $@

# Check for build errors
.PHONY: all clean
clean:
	rm -f $(TARGET)
