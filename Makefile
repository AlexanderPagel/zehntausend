# Files
SRC = $(wildcard *.cpp)
OBJ = ${SRC:.cpp=.o}
DEPS = ${SRC:.cpp=.d}
OUT = zehntausend

# Compiler and basic flags
CXX := g++
CXXFLAGS := -Wall -Werror -Wextra -Wpedantic -std=c++17 -mcmodel=medium -march=native -MMD -MP
LXXFLAGS := -lncurses

#-L/usr/lib/ ??

# Target dependant flags
DEBUGFLAGS := -O0 -g
DEBUGFLAGSL :=
RELEASEFLAGS := -Ofast -DNDEBUG
RELEASEFLAGSL := -Ofast -DNDEBUG
	# pg for profiling with gprof, -DPG to trigger fuse for early exit
PROFILEFLAGS := $(RELEASEFLAGS) -pg -DPG
PROFILEFLAGSL := $(RELEASEFLAGSL) -pg -DPG
debug: CXXFLAGS += $(DEBUGFLAGS)
debug: LXXFLAGS += $(DEBUGFLAGSL)
$(OUT): CXXFLAGS += $(RELEASEFLAGS)
$(OUT): LXXFLAGS += $(RELEASEFLAGSL)
profile: CXXFLAGS += $(PROFILEFLAGS)
profile: LXXFLAGS += $(PROFILEFLAGSL)

# Debug Target
.PHONY: debug
debug: .debug
.debug: $(OBJ)
	$(CXX) $(OBJ) $(CXXFLAGS) $(LXXFLAGS) -o $(OUT)
	touch .debug

# Profile Target
.PHONY: profile
profile: .profile
.profile: $(OBJ)
	$(CXX) $(OBJ) $(CXXFLAGS) $(LXXFLAGS) -o $(OUT)
	touch .profile

# Release Target
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
