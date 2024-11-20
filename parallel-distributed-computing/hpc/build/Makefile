CC = cc
CFLAGS = -O3
SOURCES = ../src/hpc/hpc.c \
          ../../common/matrix/matrix.c \
          ../../common/c_timer/c_timer.c \
          ../src/main.c
TARGET = hpc.out

# Default target
all: $(TARGET)

# Rule to build the target
$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $@

# Check for build errors
.PHONY: all clean
clean:
	rm -f $(TARGET)
