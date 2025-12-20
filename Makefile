CXX = g++ 
CXXFLAGS = -std=c++11 -Wall -Wextra -c -lm
TARGET = KittenSweeper



all:  $(TARGET)

$(TARGET):
	$(CXX) $(CXXFLAGS) src/*.cpp -o $(TARGET)  
debug:
	$(CXX) $(CXXFLAGS) -ggdb src/*.cpp -fsanitize=address
	$(CXX) -g  *.o libs/liblinray.a -o KittenSweeper -fsanitize=address
clean:
	rm *.o

