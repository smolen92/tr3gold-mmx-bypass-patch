#include "text.h"

Text::Text(const char* t, float y_position, SDL_Color c, TTF_Font* font) {
	strlcpy(this->text, t, 100);

	this->color = c;
	
	this->font = font;

	this->y_position = y_position;

}

void Text::set_Color(SDL_Color c) {
	this->color = c;
}

/// \cond
void Text::render(int32_t x_position, int32_t y_position, const char* text, SDL_Color color, TTF_Font* font, SDL_Renderer* renderer) {
	SDL_Surface *temp_text_surface = TTF_RenderText_Solid(font, text, color); //flawfinder: ignore
		if(temp_text_surface == NULL) return;

		SDL_Texture *temp_text_texture = SDL_CreateTextureFromSurface(renderer, temp_text_surface);
		if(temp_text_texture != NULL) {	
			SDL_Rect dst = {x_position,y_position,0,0};

			SDL_QueryTexture(temp_text_texture, NULL, NULL, &dst.w, &dst.h);

			SDL_RenderCopy(renderer, temp_text_texture, NULL, &dst); 

			SDL_DestroyTexture(temp_text_texture);
			temp_text_texture = NULL;
		}

		SDL_FreeSurface(temp_text_surface);
		temp_text_surface = NULL;
}

void Text::render(SDL_Renderer* renderer) {
	this->render(0, this->y_position, this->text, this->color, this->font, renderer); 
}
	
void Text::print() {
	fprintf(stdout, "%s\n", this->text);
}
/// \endcond

Text::~Text() {
	font = NULL;
}

