CC = mpicc
SOURCES = ../src/laplace/laplace.c \
          ../../common/mpi_proc/mpi_proc.c \
          ../../common/integer/integer.c \
          ../../common/matrix/matrix.c \
          ../../common/c_timer/c_timer.c \
          ../src/main.c
TARGET = laplace.out

# Default target
all: $(TARGET)

# Rule to build the target
$(TARGET): $(SOURCES)
	$(CC) $(SOURCES) -o $@

# Check for build errors
.PHONY: all clean
clean:
	rm -f $(TARGET)
