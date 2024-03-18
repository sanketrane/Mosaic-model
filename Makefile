# Makefile

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -Wextra -std=c++2a

# Source files
SRCS = $(wildcard src_files/*.cpp)

# Output executable
OUT = mosaic_program

# Default target
all: $(OUT)

$(OUT): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(OUT) $(SRCS)

clean:
	$(RM) $(OUT)