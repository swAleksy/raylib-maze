CXX = g++
CXXFLAGS = -Wall -std=c++17 -D_DEFAULT_SOURCE -Isrc
LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

SRC_DIR = src
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
TARGET = game

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET) $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all run clean