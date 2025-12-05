CXX = g++
CXXFLAGS = -std=c++17 -Iinclude -g -fopenmp
LDFLAGS = -no-pie -lGLEW -lGL -lGLU -lglut -fopenmp -lGL -lX11 -lXxf86vm -lXrandr -lpthread -lXi

SRC = $(wildcard src/*.cpp)
INC = -I ../common/include
LOC_LIB = ../common/linux_x86_64/libGLEW.a ../common/linux_x86_64/libglfw3.a

OBJ = $(SRC:.cpp=.o)
TARGET = bin/sph_sim

all: $(TARGET)

$(TARGET): $(OBJ) main.cpp
	$(CXX) $(CXXFLAGS) $(INC) $(OBJ) main.cpp -o $(TARGET) $(LOC_LIB) $(LDFLAGS)

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

clean:
	rm -f src/*.o $(TARGET)

run: $(TARGET)
	./$(TARGET) 100

.PHONY: all clean run