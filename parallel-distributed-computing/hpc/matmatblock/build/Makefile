CC = cc
CFLAGS = -O3
SOURCES = ../src/matmatblock/matmatblock.c \
          ../../gemm/src/gemm/gemm.c \
          ../../../common/matrix/matrix.c \
          ../../../common/c_timer/c_timer.c \
          ../src/main.c
TARGET = hpc-matmatblock.out

# Default target
all: $(TARGET)

# Rule to build the target
$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $@

# Check for build errors
.PHONY: all clean
clean:
	rm -f $(TARGET)
