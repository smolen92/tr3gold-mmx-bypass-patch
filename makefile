CXX = g++

CXXFLAGS = -Wall -g -lSDL2 -lSDL2_ttf -lSDL2_image

tr3gold-mmx-bypass.out: ./obj/main.o ./obj/text.o ./obj/button.o ./obj/scene.o ./obj/modifier.o ./obj/gui.o
	$(CXX) $(CXXFLAGS) -o tr3gold-mmx-bypass.out ./obj/main.o ./obj/text.o ./obj/button.o ./obj/scene.o ./obj/modifier.o ./obj/gui.o

tr3gold-mmx-bypass.exe: ./obj/main.o ./obj/text.o ./obj/button.o ./obj/scene.o ./obj/modifier.o ./obj/gui.o
	$(CXX) $(CXXFLAGS) -o tr3gold-mmx-bypass.exe ./obj/main.o ./obj/text.o ./obj/button.o ./obj/scene.o ./obj/modifier.o ./obj/gui.o -I"C:\Program Files\OpenSSL-Win64\include" -L"C:\Program Files\OpenSSL-Win64\lib\VC\x64\MT"

./obj/main.o: ./src/main.cpp ./obj/modifier.o ./obj/gui.o
	$(CXX) $(CXXFLAGS) -o ./obj/main.o -c ./src/main.cpp

./obj/text.o: ./src/text.h ./src/text.cpp 
	$(CXX) $(CXXFLAGS) -c ./src/text.cpp -o ./obj/text.o

./obj/button.o: ./src/button.h ./src/button.cpp ./obj/text.o
	$(CXX) $(CXXFLAGS) -c ./src/button.cpp  -o ./obj/button.o

./obj/scene.o: ./src/scene.h ./src/scene.cpp ./obj/button.o
	$(CXX) $(CXXFLAGS) -c ./src/scene.cpp -o ./obj/scene.o

./obj/modifier.o: ./src/modifier.h ./src/modifier.cpp
	$(CXX) $(CXXFLAGS) -c ./src/modifier.cpp -o ./obj/modifier.o

./obj/gui.o: ./src/gui.h ./src/gui.cpp ./obj/scene.o
	$(CXX) $(CXXFLAGS) -c ./src/gui.cpp -o ./obj/gui.o

.PHONY: doc clean clean-doc

doc:
	doxygen tr3gold-bypass.dox

clean:
	rm -f ./obj/*.o

clean-doc:
	rm -r ./doc/*
