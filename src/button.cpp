#include "button.h"

Button::Button(int x, int y, int w, int h, bool active, bool* action, const char* path, const char* text, TTF_Font* font, SDL_Renderer* renderer) {
	button_rect.x = x;
	button_rect.y = y;
	button_rect.w = w;
	button_rect.h = h;

	button_color.r = 0xFF;
	button_color.g = 0x00;
	button_color.b = 0x00;
	button_color.a = 0x7F;

	previous_state = false;
			
	this->active = active;
	
	this->action = action;

	button_text = new Text(text, 0, {0x00,0x00,0x00,0xFF}, font); 

	button_texture = IMG_LoadTexture(renderer, path);
	if(button_texture == NULL) {
		fprintf(stderr, "Error: Failed to load button's image: %s\n", path);	
	}
}
		
void Button::check_input(float mouse_x, float mouse_y, bool left_mouse_button_down) {
	if(!active) {
		button_color.r = 0x2F;
		button_color.g = 0x2F;
		button_color.b = 0x2F;

		return;
	}

	if( (mouse_x < button_rect.x) || (mouse_x > button_rect.x + button_rect.w) ||
		(mouse_y < button_rect.y) || (mouse_y > button_rect.y + button_rect.h) ) {
			button_color.r = 0xFF;
			button_color.g = 0x00;
			button_color.b = 0x00;

			previous_state = false;

			return;
	}

	if(left_mouse_button_down) {
		button_color.r = 0x00;
		button_color.g = 0x00;
		button_color.b = 0xFF;
				
		previous_state = true;
			
		return;
	}
			
	if(!left_mouse_button_down && previous_state) {
		previous_state = false;
		*action = true;
	}

	button_color.r = 0x00;
	button_color.g = 0xFF;
	button_color.b = 0x00;
			
}	

/// \cond
void Button::render(SDL_Renderer* renderer) {
	if(button_texture == NULL) return;
	SDL_RenderTexture(renderer, button_texture, NULL, &button_rect);
	SDL_SetRenderDrawColor(renderer, button_color.r, button_color.g, button_color.b, button_color.a);
	SDL_RenderFillRect(renderer, &button_rect);
	button_text->render(renderer,button_rect.x+15, button_rect.y);
}
/// \endcond

Button::~Button() {
	SDL_DestroyTexture(button_texture);
	button_texture = NULL;
	delete button_text;
	button_text = NULL;
}

