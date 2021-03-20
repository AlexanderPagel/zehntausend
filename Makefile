# Files
SRC = $(wildcard *.cpp)
OBJ = ${SRC:.cpp=.o}
DEPS = ${SRC:.cpp=.d}
OUT = zehntausend

# Compiler and flags
CXX := g++
CXXFLAGS := -Wall -Werror -Wextra -Wpedantic -std=c++17 -mcmodel=medium -march=native -MMD -MP
LXXFLAGS := -lncurses
#-L/usr/lib/ ??
DEBUGFLAGS := -O0 -g
RELEASEFLAGS := -Ofast -DNDEBUG

# Debug Target
.PHONY: debug
debug: CXXFLAGS += $(DEBUGFLAGS)
debug: .debug

.debug: $(OBJ)
	$(CXX) $(OBJ) $(CXXFLAGS) $(LXXFLAGS) -o $(OUT)
	touch .debug

# Release Target
$(OUT): CXXFLAGS += $(RELEASEFLAGS)
$(OUT): $(OBJ)
	$(CXX) $(OBJ) $(CXXFLAGS) $(LXXFLAGS) -o $(OUT)

# Object Targets
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(OXXFLAGS) -c $<

# Include Targets
-include ${SRC:.cpp=.d}

# Clean Target
.PHONY: clean
clean:
	rm -vrf $(OBJ) $(DEPS)
