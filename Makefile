# Makefile

# Default library path
LIB_PATH ?= /opt/intel/oneapi/2024.0/lib/

# Compiler
CXX = icpx 

# Compiler flags
CXXFLAGS = -Wall -Wextra -std=c++2a

# Source files
SRCS = $(filter-out src_files/main.cpp, $(wildcard src_files/*.cpp))

# Output executable
OUT = mosaic_program

# Default target
all: $(OUT)

$(OUT): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(OUT) $(SRCS) -L$(LIB_PATH) -ltbb

#$(OUT): $(SRCS)
#	$(CXX) $(CXXFLAGS) -o $(OUT) $(SRCS) -ltbb

clean:
	$(RM) $(OUT)