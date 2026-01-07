CXX = g++ 
CXXFLAGS = -std=c++11 -Wall -Wextra -c -lm
TARGET = KittenSweeper
WINDOWS_CXX = x86_64-w64-mingw32-g++  


all:  $(TARGET)

$(TARGET):
	$(CXX) $(CXXFLAGS) src/*.cpp -c 
	$(CXX) *.o libs/liblinray.a -o KittenSweeper
debug:
	$(CXX) $(CXXFLAGS) -ggdb src/*.cpp 	
	$(CXX) -g  *.o ./libs/liblinray.a -o KittenSweeper
windows:
	$(WINDOWS_CXX) $(CXXFLAGS) src/*.cpp
	$(WINDOWS_CXX) *.o ./libs/libwinray.a -o KittenSweeper.exe -lwinmm -lgdi32
clean:
	rm *.o

