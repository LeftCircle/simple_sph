CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude -g -fopenmp
LDFLAGS = -lGLEW -lGL -lGLU -lglut -fopenmp

SRC = $(wildcard src/*.cpp)
INC = -Icommon
LOC_LIB = common/linux_x86_64/libGLEW.a common/linux_x86_64/libglfw3.a

OBJ = $(SRC:.cpp=.o)
TARGET = bin/sph_sim

all: $(TARGET)

$(TARGET): $(OBJ) main.cpp
	$(CXX) $(CXXFLAGS) $(INC) $(OBJ) main.cpp -o $(TARGET) $(LDFLAGS) $(LOC_LIB)

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

clean:
	rm -f src/*.o $(TARGET)

run: $(TARGET)
	./$(TARGET) 100

.PHONY: all clean run