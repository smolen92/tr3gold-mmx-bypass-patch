#include "button.h"

Button::Button(int x, int y, int w, int h, bool active, bool* action, const char* path, const char* text, SDL_Renderer* renderer) {
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

	strncpy(this->button_text,text,100);

	button_texture = IMG_LoadTexture(renderer, path);
	if(button_texture == NULL) {
		fprintf(stderr, "Error: Failed to load button's image: %s\n", path);	
	}
}
		
void Button::check_input(float mouse_x, float mouse_y, bool left_mouse_button_down) {
	if(!active) {
		//gray color modulation
		button_color.r = 0x2F;
		button_color.g = 0x2F;
		button_color.b = 0x2F;

		return;
	}

	//no color modulation
	button_color.r = 0xFF;
	button_color.g = 0xFF;
	button_color.b = 0xFF;

	if( (mouse_x < button_rect.x) || (mouse_x > button_rect.x + button_rect.w) ||
		(mouse_y < button_rect.y) || (mouse_y > button_rect.y + button_rect.h) ) {

			previous_state = false;

			return;
	}

	if(left_mouse_button_down) {
		//blue color modulation
		button_color.r = 0x00;
		button_color.g = 0x00;
		button_color.b = 0xE0;

		previous_state = true;
			
		return;
	}
			
	if(!left_mouse_button_down && previous_state) {
		previous_state = false;
		*action = true;
	}

	//green color modulation
	button_color.r = 0x00;
	button_color.g = 0xE0;
	button_color.b = 0x00;
			
}	

/// \cond
void Button::render(SDL_Renderer* renderer, TTF_Font* font) {
	if(button_texture == NULL) return;
	SDL_SetTextureColorMod(button_texture, button_color.r, button_color.g, button_color.b);
	SDL_RenderCopy(renderer, button_texture, NULL, &button_rect);
	Text::render(button_rect.x+15, button_rect.y, this->button_text, {0,0,0,0xFF}, font, renderer);
}
/// \endcond

Button::~Button() {
	SDL_DestroyTexture(button_texture);
	button_texture = NULL;
}

