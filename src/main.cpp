#include <stdio.h>

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "button.h"
#include "text.h"
#include "scene.h"
#include "modifier.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 200

/// \todo connect modifier to button
/// \todo create scenes
/// \todo create cli
/// \todo test the program
/// \todo sdlerror to stderr?

int main(int argc, char **argv) {
	//target and replace string
	const uint8_t target[] = { 0x89, 0x15, 0x50, 0x7c, 0x6c, 0x00, //mov dword ptr [DAT_006c7c50], EDX
				0x5b, // POP EBX
				0x8b, 0xe5, //MOV ESP,EBP
				0x5d, //POP EBP
				0xc3, //RET
				0x90, //NOP
				0x90,
				0x90, 
				0x90,
				0x90,
				0x90,
				0x90,
				0x90,
				0x90,
				0x90,
				0x90, 
				0x90,
				0x90
				}; 
	
	const uint8_t replace[] = { 0xc7, 0x05, 0x50, 0x7c, 0x6c, 0x00, 0x00, 0x00, 0x00, 0x00, //MOV dword ptr[DAT_006c7c50], 0x00
				0x5b, //POP EBX
				0x8b, 0xe5, //MOV ESP, EBP
				0x5d, //POP EBP
				0xc3, //RET
				};
	
	const int32_t target_size = 24;
	const int32_t replace_size = 15;
	
	const char compatible_md5sum[] = "7c820c372f3ca0b7e97e09cc91a0f033";
	
	Modifier tr3gold_modifier;
	
	/// \todo check for errors
	tr3gold_modifier.load_files("tr3gold.exe", "tr3gold.bak");

	//Init
	if(!SDL_Init(SDL_INIT_VIDEO|SDL_INIT_EVENTS) ) {
		printf("Error: %s\n", SDL_GetError());
		return 1;
	}

	if(!TTF_Init()) {
		SDL_Log("Error: %s\n", SDL_GetError());
		return 1;
	}

	SDL_Renderer* renderer;
	SDL_Window* window;

	window = SDL_CreateWindow("Patch GUI", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_VULKAN);
	if( window == NULL) {
		printf("Error: %s\n", SDL_GetError());
	}

	renderer = SDL_CreateRenderer(window, NULL);
	if(renderer == NULL) {
		printf("Error: %s\n", SDL_GetError());
	}
	
	TTF_Font* font = TTF_OpenFont("./assets/Montserrat-Regular.ttf",FONT_SIZE);
	if( font == NULL ) {
		printf("Error: %s\n", SDL_GetError());
		return 1;
	}

	SDL_Color white_color = {0xFF,0xFF,0xFF,0xFF};
	SDL_Color red_color = {0xFF, 0x00, 0x00, 0xFF};
	SDL_Color green_color = {0x00, 0xFF, 0x00, 0xFF};

	bool running = true;

	bool left_mouse_button_down = false;

	//void (*fun_pointer)(int);

	//fun_pointer = &test;

	Scene testScene;

	testScene.add_button(new Button(600,100, 100,50,true));
	testScene.add_text("Test", white_color);

	//main loop
	while(running) {
		
		//input
		SDL_Event input;

		while(SDL_PollEvent(&input)) {
			if(input.type == SDL_EVENT_QUIT) {
				running = false;
			}

			if(input.button.type == SDL_EVENT_MOUSE_BUTTON_DOWN && input.button.button == 1) {
				left_mouse_button_down = true;
			}

			if(input.button.type == SDL_EVENT_MOUSE_BUTTON_UP && input.button.button == 1) {
				left_mouse_button_down = false;
			}
		}
		
		float mouse_x, mouse_y;
		SDL_GetMouseState(&mouse_x, &mouse_y);
		
		//logic
		testScene.check_input(mouse_x, mouse_y, left_mouse_button_down);
		
		//rendering
		SDL_SetRenderDrawColor(renderer, 0,0,0, 0xFF);
		SDL_RenderClear(renderer);
	
		testScene.render(renderer, font);

		SDL_RenderPresent(renderer);
	}
	

	//clean
	SDL_DestroyRenderer(renderer);
	renderer = NULL;

	SDL_DestroyWindow(window);
	window = NULL;

	TTF_Quit();
	SDL_Quit();

	return 0;
}

