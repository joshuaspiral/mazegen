CXX=clang++
CXXFLAGS=-Wall -Wextra -ggdb -O3 -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

make all: lines.cpp grid.cpp
	$(CXX) $(CXXFLAGS) -o grid grid.cpp -lm
	$(CXX) $(CXXFLAGS) -o lines lines.cpp -lm

grid: grid.cpp
	$(CXX) $(CXXFLAGS) -o grid grid.cpp -lm

lines: lines.cpp
	$(CXX) $(CXXFLAGS) -o lines lines.cpp -lm

clean:
	rm grid lines
