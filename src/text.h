#ifndef _TEXT_H_
#define _TEXT_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef SDL_VER2
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_ttf.h>
#endif

#ifdef SDL_VER3
	#include <SDL3/SDL.h>
	#include <SDL3_ttf/SDL_ttf.h>
#endif

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

		/**
		 * @brief render the text on screen
		 *
		 * @param x_position x postion of the text
		 * @param y_position y position of the text
		 * @param text pointer to text to render
		 * @param color color of the text
		 * @param font text's font
		 * @param renderer renderer to render the text to
		 *
		 */
		static void render(int32_t x_position, int32_t y_position, const char* text, SDL_Color color, TTF_Font* font, SDL_Renderer* renderer);
	
		/// \cond
		void render(SDL_Renderer* renderer);

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

