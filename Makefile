CXX := g++
CXXFLAGS := -O2 -std=c++17 -Wall -Wextra -Wpedantic -I./src


SRC := $(wildcard src/*.cpp) \
	$(wildcard src/algorithms/*.cpp) \
	$(wildcard src/simulation/*.cpp) \
	$(wildcard src/utils/*.cpp)


OBJ := $(SRC:.cpp=.o)
BIN := build
EXE := $(BIN)/disaster_sim


.PHONY: all clean run tests


all: $(EXE)


$(EXE): $(OBJ)
	@mkdir -p $(BIN)
	$(CXX) $(CXXFLAGS) -o $@ $^


clean:
	rm -rf $(BIN) $(OBJ)


run: all
	$(EXE) tests/grid_5x5.txt tests/resources_basic.txt tests/incidents_basic.txt