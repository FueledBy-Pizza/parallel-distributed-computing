CC = mpicc
CFLAGS = -O3
SOURCES = ../src/cascadesum/cascadesum.c \
          ../../common/integer/integer.c \
          ../../common/vector/vector.c \
          ../../common/c_timer/c_timer.c \
          ../src/main.c
TARGET = cascadesum.out

# Default target
all: $(TARGET)

# Rule to build the target
$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $@

# Check for build errors
.PHONY: all clean
clean:
	rm -f $(TARGET)
