#include "gui.h"


int Gui::gui_init(TTF_Font** font) {

	#ifdef SDL_VER2
		if( SDL_Init(SDL_INIT_VIDEO|SDL_INIT_EVENTS) < -1 ) {
	#endif

	#ifdef SDL_VER3
		if(!SDL_Init(SDL_INIT_VIDEO|SDL_INIT_EVENTS) ) {
	#endif
			fprintf(stderr, "Error: %s\n", SDL_GetError());
			return 1;
		}

	#ifdef SDL_VER2
		if( TTF_Init() < -1 ) {
	#endif

	#ifdef SDL_VER3
		if(!TTF_Init()) {
	#endif
			fprintf(stderr, "Error: %s\n", SDL_GetError());
			return 1;
		}
	#ifdef SDL_VER2
		window = SDL_CreateWindow("Patch GUI", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_VULKAN);
	#endif

	#ifdef SDL_VER3
		window = SDL_CreateWindow("Patch GUI", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	#endif
	if( window == NULL) {
		fprintf(stderr, "Error: %s\n", SDL_GetError());
		return 1;
	}

	#ifdef SDL_VER2
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
	#endif

	#ifdef SDL_VER3
		renderer = SDL_CreateRenderer(window, NULL);
	#endif
	if(renderer == NULL) {
		fprintf(stderr, "Error: %s\n", SDL_GetError());
		return 1;
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
		
		#ifdef SDL_VER2
			if(input.type == SDL_QUIT) {
		#endif

		#ifdef SDL_VER3
			if(input.type == SDL_EVENT_QUIT) {
		#endif
				*running = false;
			}

		#ifdef SDL_VER2
			if(input.button.type == SDL_MOUSEBUTTONDOWN && input.button.button == 1) {
		#endif

		#ifdef SDL_VER3
			if(input.button.type == SDL_EVENT_MOUSE_BUTTON_DOWN && input.button.button == 1) {
		#endif
				left_mouse_button_down = true;
			}

		#ifdef SDL_VER2
			if(input.button.type == SDL_MOUSEBUTTONUP && input.button.button == 1) {
		#endif

		#ifdef SDL_VER3
			if(input.button.type == SDL_EVENT_MOUSE_BUTTON_UP && input.button.button == 1) {
		#endif
				left_mouse_button_down = false;
			}
	}
		
	SDL_GetMouseState(&mouse_x, &mouse_y);
		
}

void Gui::logic() {
	current_scene->check_input(mouse_x, mouse_y, left_mouse_button_down);
}

void Gui::render(TTF_Font* font) {
	SDL_SetRenderDrawColor(renderer, 0,0,0, 0xFF);
	SDL_RenderClear(renderer);
	
	current_scene->render(renderer, font);

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

