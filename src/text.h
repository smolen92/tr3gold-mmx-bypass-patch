#ifndef _TEXT_H_
#define _TEXT_H_

#include <string.h>
#include <stdlib.h>

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

class Text {
	public:
		/**
		 * @param t - text that will be rendered
		 * @param c - color of the text
		 *
		 * @return 0 - on success
		 * @return 1 - on failure
		 */
		int set_text(const char* t, SDL_Color c);
		
		/**
		 * @param c - color of the text
		 */
		void set_Color(SDL_Color c);
		
		/**
		 * @param renderer - renderer that is used for drawing the button
		 * @param font - font that is used
		 */
		void render(SDL_Renderer* renderer, TTF_Font* font);

		void free_text();
	private:
		char* text;
		SDL_Color color;
};

#endif

