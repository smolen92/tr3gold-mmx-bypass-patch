#include "text.h"

Text::Text(const char* t, float y_position, SDL_Color c, TTF_Font* font) {
	strcpy(this->text, t);

	this->color = c;
	
	this->font = font;

	this->y_position = y_position;

}

void Text::set_Color(SDL_Color c) {
	this->color = c;
}

/// \cond
void Text::render(SDL_Renderer* renderer) {
	SDL_Surface *temp_text_surface = TTF_RenderText_Solid(font, this->text, strlen(this->text), this->color);
		if(temp_text_surface == NULL) return;

		SDL_Texture *temp_text_texture = SDL_CreateTextureFromSurface(renderer, temp_text_surface);
		if(temp_text_texture != NULL) {	
			SDL_FRect dst = {0,this->y_position,0,0};

			SDL_GetTextureSize(temp_text_texture, &dst.w, &dst.h);

			SDL_RenderTexture(renderer, temp_text_texture, NULL, &dst);

			SDL_DestroyTexture(temp_text_texture);
			temp_text_texture = NULL;
		}

		SDL_DestroySurface(temp_text_surface);
		temp_text_surface = NULL;
}
/// \endcond

Text::~Text() {
	font = NULL;
}

