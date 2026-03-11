CXX = g++
CXXFLAGS = -Wall -std=c++17 -D_DEFAULT_SOURCE
LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

SRCS = main.cpp Grid.cpp MazeGenerator.cpp

all: game

game: $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o game $(LDFLAGS)

run: game
	./game

clean:
	rm -f game