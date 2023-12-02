# Makefile for compiling intelmkltest.c with icc and linking with MKL

# Compiler
CC=icc

# Output executable name
TARGET=saxpy_mkl

# Source files
SOURCES=saxpy_mkl.cpp

# Include directories
INCLUDES=-I$(MKLROOT)/include -I../../common

# Linker flags
LDFLAGS=-L$(MKLROOT)/lib/intel64 -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core -liomp5 -lpthread -lm -ldl


# Compiler flags
CFLAGS=-qmkl -O2 -Wall

# Build rule
all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(INCLUDES) $(CFLAGS) $(SOURCES) -o $(TARGET) $(LDFLAGS)

# Clean rule
clean:
	rm -f $(TARGET)

.PHONY: all clean
