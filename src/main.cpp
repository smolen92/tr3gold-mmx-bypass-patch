#include "scene.h"
#include "modifier.h"

/// \cond
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 200
/// \endcond

/// \file
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
	
	Modifier tr3gold_modifier;
	
	Scene testScene;
	
	testScene.add_text(new Text("Tr3gold MMX bypass patch", 0, white_color, font));
	
	int files_status = tr3gold_modifier.load_files("tr3gold.exe", "tr3gold.bak");
	if(files_status != 0) {
		if( files_status & ERROR_INPUT_FILE_NOT_FOUND) testScene.add_text(new Text("Error: Input file not found", FONT_SIZE, red_color, font));
		if( files_status & ERROR_CANNOT_OPEN_BACKUP_FILE) testScene.add_text(new Text("Error: Cannot open backup file", 2*FONT_SIZE, red_color, font));
		if( files_status & ERROR_UNKNOWN_MESSAGE_DIGEST_MD5) testScene.add_text(new Text("Error: Unknown message digest md5", 3*FONT_SIZE, red_color, font));
		if( files_status & ERROR_MESSAGE_DIGEST_CREATE_FAILED) testScene.add_text(new Text("Error: Digest create failed", 3*FONT_SIZE, red_color, font));
		if( files_status & ERROR_MESSAGE_DIGEST_INITIALIZATION_FAILED) testScene.add_text(new Text("Error: Digest initialization failed", 3*FONT_SIZE, red_color, font));
		if( files_status & ERROR_MESSAGE_DIGEST_UPDATE_FAILED) testScene.add_text(new Text("Error: Message digest update failed", 3*FONT_SIZE, red_color, font));
		if( files_status & ERROR_MESSAGE_DIGEST_FINALIZATION_FAILED) testScene.add_text(new Text("Error: Message digest finalization failed", 3*FONT_SIZE, red_color, font));
		testScene.add_button(new Button(600,100, 100,50,false));
	}
	else {
		testScene.add_button(new Button(600,100,100,50,true));
	}

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
	
		testScene.render(renderer);

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

