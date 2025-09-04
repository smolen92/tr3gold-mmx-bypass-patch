#ifndef _TEXT_H_
#define _TEXT_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

/**
 * @brief class for handling displayed text
 */
class Text {
	public:
		/**
		 * @param text - text that will be rendered
		 * @param y-pointer y position of the text
		 * @param color - color of the text
		 * @param font - pointer to font that will be used
		 *
		 * @return 0 - on success
		 * @return 1 - on failure
		 */
		Text(const char* text, float y_position, SDL_Color color, TTF_Font* font);
		
		/**
		 * @param c - color of the text
		 */
		void set_Color(SDL_Color c);
	
		/// \cond
		void render(SDL_Renderer* renderer);
		/// \endcond

		~Text();
	private:
		char text[100];
		SDL_Color color;
		TTF_Font* font;
		float y_position;
};

#endif

