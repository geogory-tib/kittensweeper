CXX = c++ 
CXXFLAGS = -std=c++11 -Wall -Wextra -lm -L/opt/homebrew/lib -I/opt/homebrew/include/ -lraylib
TARGET = KittenSweeper



all:  $(TARGET)

$(TARGET):
	$(CXX) $(CXXFLAGS) src/*.cpp -o $(TARGET)  
debug:
	$(CXX) $(CXXFLAGS) -g src/*.cpp -o $(TARGET) 
clean:
	rm KittenSweeper

