PLATFORM=linux
SDL_VER=SDL3
MODE=debug
OUTFILE=tr3gold-mmx-bypass.out


CXX = g++

CXXFLAGS = -Wall 


ifeq ($(MODE), debug) 
	CXXFLAGS += -g
endif


ifeq ($(PLATFORM), windows)
	CXXFLAGS += -I"C:\SDL3\i686-w64-mingw32\include" -L"C:\SDL3\i686-w64-mingw32\lib" -static-libgcc -static-libstdc++
	OUTFILE = tr3gold-mmx-bypass.exe
endif


ifeq ($(SDL_VER), SDL2)
	SDLFLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf
	CXXFLAGS += -DSDL_VER2
else
	SDLFLAGS = -lSDL3 -lSDL3_image -lSDL3_ttf
	CXXFLAGS += -DSDL_VER3
endif


$(OUTFILE): ./obj/main.o ./obj/text.o ./obj/button.o ./obj/scene.o ./obj/modifier.o ./obj/gui.o
	$(CXX) $(CXXFLAGS) -o $(OUTFILE) ./obj/main.o ./obj/text.o ./obj/button.o ./obj/scene.o ./obj/modifier.o ./obj/gui.o $(SDLFLAGS) 

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

clean-windows:
	del ./obj/*.o
