CXX = g++

CXXFLAGS = -Wall -g -lSDL3 -lSDL3_ttf -lcrypto

a.out: ./obj/main.o ./obj/text.o ./obj/button.o ./obj/scene.o ./obj/modifier.o
	$(CXX) $(CXXFLAGS) -o a.out ./obj/main.o ./obj/text.o ./obj/button.o ./obj/scene.o ./obj/modifier.o

./obj/main.o: ./src/main.cpp ./obj/scene.o ./obj/text.o ./obj/button.o
	$(CXX) $(CXXFLAGS) -o ./obj/main.o -c ./src/main.cpp

./obj/text.o: ./src/text.h ./src/text.cpp 
	$(CXX) $(CXXFLAGS) -c ./src/text.cpp -o ./obj/text.o

./obj/button.o: ./src/button.h ./src/button.cpp
	$(CXX) $(CXXFLAGS) -c ./src/button.cpp  -o ./obj/button.o

./obj/scene.o: ./src/scene.h ./src/scene.cpp ./obj/text.o ./obj/button.o
	$(CXX) $(CXXFLAGS) -c ./src/scene.cpp -o ./obj/scene.o

./obj/modifier.o: ./src/modifier.h ./src/modifier.cpp
	$(CXX) $(CXXFLAGS) -c ./src/modifier.cpp -o ./obj/modifier.o

