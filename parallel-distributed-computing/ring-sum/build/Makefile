CC = mpicc
SOURCES = ../src/ring-sum/ring-sum.c \
          ../../common/mpi_proc/mpi_proc.c \
          ../../common/integer/integer.c \
          ../../common/vector/vector.c \
          ../../common/mpi_proc/mpi_proc_vector/mpi_proc_vector.c \
          ../src/main.c
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
