#ifndef _TEXT_H_
#define _TEXT_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#define FONT_SIZE 28

/**
 * @brief class for handling displayed text
 */
class Text {
	public:
		/**
		 * @param text - text that will be rendered
		 * @param y_position y position of the text
		 * @param color - color of the text
		 * @param font - pointer to font that will be used
		 */
		Text(const char* text, float y_position, SDL_Color color, TTF_Font* font);
		
		/**
		 * @param c - color of the text
		 */
		void set_Color(SDL_Color c);

		/// \todo re-do text rendering
		void render(SDL_Renderer* renderer, int32_t x_position = 0, int32_t y_position = -1);
		
		/// \cond
		void print();
		/// \endcond

		~Text();
	private:
		char text[100]; //flawfinder: ignore
		SDL_Color color;
		TTF_Font* font;
		float y_position;
};

#endif

