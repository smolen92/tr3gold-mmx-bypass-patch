#include "scene.h"
#include "modifier.h"

/// \cond
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 200
/// \endcond

/// \file
/// \todo get rid of magic numbers when creating scene
/// \todo create cli
/// \todo test the program

/// \cond
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
		fprintf(stderr, "Error: %s\n", SDL_GetError());
		return 1;
	}

	if(!TTF_Init()) {
		fprintf(stderr, "Error: %s\n", SDL_GetError());
		return 1;
	}

	SDL_Renderer* renderer;
	SDL_Window* window;

	window = SDL_CreateWindow("Patch GUI", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_VULKAN);
	if( window == NULL) {
		fprintf(stderr, "Error: %s\n", SDL_GetError());
	}

	renderer = SDL_CreateRenderer(window, NULL);
	if(renderer == NULL) {
		fprintf(stderr, "Error: %s\n", SDL_GetError());
	}
	
	TTF_Font* font = TTF_OpenFont("./assets/Montserrat-Regular.ttf",FONT_SIZE);
	if( font == NULL ) {
		fprintf(stderr, "Error: %s\n", SDL_GetError());
		return 1;
	}

	SDL_Color white_color = {0xFF,0xFF,0xFF,0xFF};
	SDL_Color red_color = {0xFF, 0x00, 0x00, 0xFF};
	SDL_Color green_color = {0x00, 0xFF, 0x00, 0xFF};
	SDL_Color yellow_color = {0xFF,0xFF,0x00,0xFF};

	bool running = true;

	bool left_mouse_button_down = false;
	
	Modifier tr3gold_modifier;
	
	Scene main_scene, promt_scene;
	Scene* current_scene = &main_scene;

	main_scene.add_text(new Text("Tr3gold MMX bypass patch", 0, white_color, font));
	
	int files_status = tr3gold_modifier.load_files("tr3gold.exe", "tr3gold.bak", compatible_md5sum);

	bool patch_button_state = true;

	bool patch_button_pressed = false;
	bool promt_yes_button_pressed = false;
	bool promt_no_button_pressed = false;

	if(files_status != 0) {
		if( files_status & ERROR_INPUT_FILE_NOT_FOUND) {
			main_scene.add_text(new Text("Error: Input file not found", FONT_SIZE, red_color, font));
			patch_button_state = false;
		}

		if( files_status & ERROR_CANNOT_OPEN_BACKUP_FILE) {
			promt_scene.add_text(new Text("Error: Cannot open backup file", 0, red_color, font));
		}

		if( files_status & ERROR_UNKNOWN_MESSAGE_DIGEST_MD5) {
			promt_scene.add_text(new Text("Error: Unknown message digest md5", 2*FONT_SIZE, red_color, font));
		}

		if( files_status & ERROR_MESSAGE_DIGEST_CREATE_FAILED) {
			promt_scene.add_text(new Text("Error: Digest create failed", 2*FONT_SIZE, red_color, font));
		}

		if( files_status & ERROR_MESSAGE_DIGEST_INITIALIZATION_FAILED) {
			main_scene.add_text(new Text("Error: Digest initialization failed", 2*FONT_SIZE, red_color, font));
		}

		if( files_status & ERROR_MESSAGE_DIGEST_UPDATE_FAILED) {
			promt_scene.add_text(new Text("Error: Message digest update failed", 2*FONT_SIZE, red_color, font));
		}

		if( files_status & ERROR_MESSAGE_DIGEST_FINALIZATION_FAILED) {
			promt_scene.add_text(new Text("Error: Message digest finalization failed", 2*FONT_SIZE, red_color, font));
		}
	
		if( files_status & ERROR_MD5_SUM_DOESNT_MATCH) {
			promt_scene.add_text(new Text("Error: Md5 checksum doesn't match", 3*FONT_SIZE, red_color, font));
		}

		main_scene.add_text(new Text("Warning: Some test didn't pass", 2*FONT_SIZE, yellow_color, font));
		main_scene.add_button(new Button(600,100, 100,50, patch_button_state, &patch_button_pressed));
		promt_scene.add_button(new Button(450, 100, 100, 50, true, &promt_no_button_pressed));
		promt_scene.add_button(new Button(600, 100, 100, 50, true, &promt_yes_button_pressed));
		promt_scene.add_text(new Text("Try to patch the file anyway?", 4*FONT_SIZE, white_color, font));
	}
	else {
		main_scene.add_button(new Button(600,100,100,50,true,&patch_button_pressed));
		main_scene.add_text(new Text("Every test passed", FONT_SIZE, green_color, font));
	}


	//main loop
	while(running) {
	
		patch_button_pressed = false;
		promt_no_button_pressed = false;
		promt_yes_button_pressed = false;

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
		current_scene->check_input(mouse_x, mouse_y, left_mouse_button_down);
	
		if( (patch_button_pressed && (files_status == 0) ) || promt_yes_button_pressed ) {
			int modifier_return_value = tr3gold_modifier.replace_bytes(target, target_size, replace, replace_size);
			
			if(modifier_return_value != 0) {
				main_scene.clear();
				main_scene.add_text(new Text("Error while patching the file", 0, red_color, font));
			
				if( modifier_return_value & ERROR_READING_INPUT_FILE) main_scene.add_text(new Text("Error: while reading input file", FONT_SIZE, red_color, font));
				if( modifier_return_value & ERROR_MULTIPLE_TARGET_LOCATION) main_scene.add_text(new Text("Error: Multiple target location found", FONT_SIZE, red_color, font));
				if( modifier_return_value & ERROR_WRITING_BACKUP_FILE) main_scene.add_text(new Text("Error: while creating backup file", FONT_SIZE, red_color, font));
				if( modifier_return_value & ERROR_TARGET_NOT_FOUND) main_scene.add_text(new Text("Error: Target bytes were not found", FONT_SIZE, red_color, font));
				if( modifier_return_value & ERROR_MODIFYING_FILE) main_scene.add_text(new Text("Error: Couldn't modify the file", FONT_SIZE, red_color, font));
				
				current_scene = &main_scene;
			}
			else {
				main_scene.clear();
				main_scene.add_text(new Text("Sucess, the file was patched", 0, green_color, font));
				current_scene = &main_scene;
			}

		}

		if( patch_button_pressed && (files_status != 0) ) {
			current_scene = &promt_scene;
		}

		if(promt_no_button_pressed) {
			current_scene = &main_scene;
		}

		//rendering
		SDL_SetRenderDrawColor(renderer, 0,0,0, 0xFF);
		SDL_RenderClear(renderer);
	
		current_scene->render(renderer);

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
/// \endcond

