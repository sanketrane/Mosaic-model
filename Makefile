# Makefile

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -Wextra -std=c++2a

# Source files
SRCS = main.cpp Subpop.cpp accessory_functions.cpp procedural_functions.cpp doSim_functions.cpp

# Output executable
OUT = mosaic_program

# Default target
all: $(OUT)

$(OUT): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(OUT) $(SRCS)

clean:
	$(RM) $(OUT)