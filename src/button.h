#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <stdio.h>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "text.h"

/**
 * @brief button class
 */
class Button {
	public:
	
		/**
		 * @param x - x coordinate of button
		 * @param y - y coordinate of button
		 * @param w - width of the button
		 * @param h - height of the button
		 * @param active - true if button should receive input, false if not
		 * @param action - pointer to bool that is set to true if button was pressed
		 * @param path - filesystem path to button image
		 * @param renderer - renderer that will be used
		 */
		Button(int x, int y, int w, int h, bool active, bool *action, const char* path, const char* text, TTF_Font* font, SDL_Renderer* renderer);
		
		/**
		 * @brief check if button is pressed and trigger its function
		 *
		 * @param mouse_x - current x coordinate of mouse posistion
		 * @param mouse_y - current y coordinate of mouse position
		 * @param left_mouse_button_down - true if LMB is pressed, false otherwise
		 */
		void check_input(float mouse_x, float mouse_y, bool left_mouse_button_down);
	
		~Button();

		/// \cond
		void render(SDL_Renderer* renderer);
		/// \endcond
	private:
		SDL_FRect button_rect;
		SDL_Color button_color;
		bool previous_state;
		bool active;
		bool* action;
		SDL_Texture* button_texture;
		Text* button_text;			
};

#endif

