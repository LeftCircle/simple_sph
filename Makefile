CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude -g -fopenmp
LDFLAGS = -lGLEW -lGL -lGLU -lglut -fopenmp

SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:.cpp=.o)
TARGET = bin/sph_sim

all: $(TARGET)

$(TARGET): $(OBJ) main.cpp
	$(CXX) $(CXXFLAGS) $(OBJ) main.cpp -o $(TARGET) $(LDFLAGS)

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f src/*.o $(TARGET)

run: $(TARGET)
	./$(TARGET) 100

.PHONY: all clean run