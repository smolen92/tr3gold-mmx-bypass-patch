#include "gui.h"


int Gui::gui_init(TTF_Font** font) {
	if(!SDL_Init(SDL_INIT_VIDEO|SDL_INIT_EVENTS) ) {
		fprintf(stderr, "Error: %s\n", SDL_GetError());
		return 1;
	}

	if(!TTF_Init()) {
		fprintf(stderr, "Error: %s\n", SDL_GetError());
		return 1;
	}

	window = SDL_CreateWindow("Patch GUI", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_VULKAN);
	if( window == NULL) {
		fprintf(stderr, "Error: %s\n", SDL_GetError());
	}

	renderer = SDL_CreateRenderer(window, NULL);
	if(renderer == NULL) {
		fprintf(stderr, "Error: %s\n", SDL_GetError());
	}

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	*font = TTF_OpenFont("./assets/Montserrat-Regular.ttf",FONT_SIZE);
	if( font == NULL ) {
		fprintf(stderr, "Error: %s\n", SDL_GetError());
		return 1;
	}

	current_scene = NULL;

	left_mouse_button_down = false;

	return 0;
}

void Gui::input(bool *running) {
	SDL_Event input;

	while(SDL_PollEvent(&input)) {
		if(input.type == SDL_EVENT_QUIT) {
			*running = false;
		}

		if(input.button.type == SDL_EVENT_MOUSE_BUTTON_DOWN && input.button.button == 1) {
			left_mouse_button_down = true;
		}

		if(input.button.type == SDL_EVENT_MOUSE_BUTTON_UP && input.button.button == 1) {
			left_mouse_button_down = false;
		}
	}
		
	
	SDL_GetMouseState(&mouse_x, &mouse_y);
		

}

void Gui::logic() {
	current_scene->check_input(mouse_x, mouse_y, left_mouse_button_down);
}

void Gui::render() {
	SDL_SetRenderDrawColor(renderer, 0,0,0, 0xFF);
	SDL_RenderClear(renderer);
	
	current_scene->render(renderer);

	SDL_RenderPresent(renderer);

}

void Gui::print() {
	current_scene->print();
}

Gui::~Gui() {
	SDL_DestroyRenderer(renderer);
	renderer = NULL;

	SDL_DestroyWindow(window);
	window = NULL;

	TTF_Quit();
	SDL_Quit();
}

