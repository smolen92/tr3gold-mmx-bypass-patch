CXX = g++

CXXFLAGS = -Wall -g -lSDL3 -lSDL3_ttf

a.out: ./obj/gui.o ./obj/text.o ./obj/button.o ./obj/scene.o
	$(CXX) $(CXXFLAGS) -o a.out ./obj/gui.o ./obj/text.o ./obj/button.o ./obj/scene.o

./obj/gui.o: ./src/gui.cpp ./obj/scene.o ./obj/text.o ./obj/button.o
	$(CXX) $(CXXFLAGS) -o ./obj/gui.o -c ./src/gui.cpp

./obj/text.o: ./src/text.h ./src/text.cpp 
	$(CXX) $(CXXFLAGS) -c ./src/text.cpp -o ./obj/text.o

./obj/button.o: ./src/button.h ./src/button.cpp
	$(CXX) $(CXXFLAGS) -c ./src/button.cpp  -o ./obj/button.o

./obj/scene.o: ./src/scene.h ./src/scene.cpp ./obj/text.o ./obj/button.o
	$(CXX) $(CXXFLAGS) -c ./src/scene.cpp -o ./obj/scene.o

