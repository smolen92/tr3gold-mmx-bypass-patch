#include <stdio.h>
#include <SDL3/SDL.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 200

struct color {
	int red;
	int green;
	int blue;
};

class Button {
	public:
		Button(int x, int y, int w, int h) {
			button_rect.x = x;
			button_rect.y = y;
			button_rect.w = w;
			button_rect.h = h;

			button_color.red = 0xFF;
			button_color.green = 0x00;
			button_color.blue = 0x00;
			previous_state = false;
			test_var = 0;
		}
		
		void render(SDL_Renderer* renderer) {
			SDL_SetRenderDrawColor(renderer, button_color.red, button_color.green, button_color.blue, 0xFF);
			SDL_RenderFillRect(renderer, &button_rect);
		}
		

		void check_input(float mouse_x, float mouse_y, bool left_mouse_button_down, void (*fun)(int a)) {
			if( (mouse_x < button_rect.x) || (mouse_x > button_rect.x + button_rect.w) ||
				(mouse_y < button_rect.y) || (mouse_y > button_rect.y + button_rect.h) ) {
					button_color.red = 0xFF;
					button_color.green = 0x00;
					button_color.blue = 0x00;

					previous_state = false;

					return;
			}

			if(left_mouse_button_down) {
				button_color.red = 0x00;
				button_color.green = 0x00;
				button_color.blue = 0xFF;
				
				previous_state = true;
				return;
			}
			
			if(!left_mouse_button_down && previous_state) {
				previous_state = false;
				fun(test_var++);
			}

			button_color.red = 0x00;
			button_color.green = 0xFF;
			button_color.blue = 0x00;
			
		}

	private:
		SDL_FRect button_rect;
		color button_color;
		bool previous_state;
		int test_var;
};

void test(int a) {
	printf("%d\n", a);
}

int main(int argc, char **argv) {

	if(!SDL_Init(SDL_INIT_VIDEO|SDL_INIT_EVENTS) ) {
		printf("Error: %s\n", SDL_GetError());
	}

	SDL_Renderer* renderer;
	SDL_Window* window;
	
	Button but(600,100, 100,50);

	window = SDL_CreateWindow("Patch GUI", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_VULKAN);
	if( window == NULL) {
		printf("Error: %s\n", SDL_GetError());
	}

	renderer = SDL_CreateRenderer(window, NULL);
	if(renderer == NULL) {
		printf("Error: %s\n", SDL_GetError());
	}

	bool running = true;

	bool left_mouse_button_down = false;

	void (*fun_pointer)(int);

	fun_pointer = &test;

	while(running) {

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

		but.check_input(mouse_x, mouse_y, left_mouse_button_down, fun_pointer);
		
		SDL_SetRenderDrawColor(renderer, 0,0,0, 0xFF);
		SDL_RenderClear(renderer);
		
		but.render(renderer);

		SDL_RenderPresent(renderer);
	}

	SDL_DestroyRenderer(renderer);
	renderer = NULL;

	SDL_DestroyWindow(window);
	window = NULL;

	SDL_Quit();

	return 0;
}

