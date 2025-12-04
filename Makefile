CXX = c++
CXXFLAGS = -Wall -Wextra -lm
TARGET = KittenSweeper



all:  $(TARGET)

$(TARGET):
	$(CXX) $(CXXFLAGS) src/*.cpp -o $(TARGET) /home/john/lib/libraylib.a
debug:
	$(CXX) $(CXXFLAGS) -g src/*.cpp -o $(TARGET) /home/john/lib/libraylib.a
clean:
	rm KittenSweeper

