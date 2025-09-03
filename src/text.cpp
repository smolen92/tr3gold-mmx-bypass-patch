#include "text.h"

int Text::set_text(const char* t, SDL_Color c) {
	this->text = (char*)malloc(strlen(t)*sizeof(char));	
	if(this->text == NULL) {
		return 1;
	}
	
	strcpy(this->text, t);

	this->color = c;

	return 0;
}

void Text::set_Color(SDL_Color c) {
	this->color = c;
}

void Text::render(SDL_Renderer* renderer, TTF_Font* font ) {
	SDL_Surface *temp_text_surface = TTF_RenderText_Solid(font, this->text, 4, this->color);
		if(temp_text_surface == NULL) return;

		SDL_Texture *temp_text_texture = SDL_CreateTextureFromSurface(renderer, temp_text_surface);
		if(temp_text_texture != NULL) {	
			SDL_FRect dst = {0,0,0,0};

			SDL_GetTextureSize(temp_text_texture, &dst.w, &dst.h);

			SDL_RenderTexture(renderer, temp_text_texture, NULL, &dst);

			SDL_DestroyTexture(temp_text_texture);
			temp_text_texture = NULL;
		}

		SDL_DestroySurface(temp_text_surface);
		temp_text_surface = NULL;
}

void Text::free_text() {
	if(this->text != NULL) {
		free(this->text);
		this->text = NULL;
	}
}

