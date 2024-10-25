CC = mpicc
SOURCES = ../src/ring-sum/ring-sum.c \
          ../../common/mpiproc/mpi_proc.c \
          ../../common/integer/integer.c \
          ../../common/vector_ops/vector_ops.c \
          ../../common/mpiproc/vector/vector.c \
          ../main.c
TARGET = ring-sum.out

# Default target
all: $(TARGET)

# Rule to build the target
$(TARGET): $(SOURCES)
	$(CC) $(SOURCES) -o $@

# Check for build errors
.PHONY: all clean
clean:
	rm -f $(TARGET)
