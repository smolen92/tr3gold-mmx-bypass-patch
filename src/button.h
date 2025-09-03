#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <SDL3/SDL.h>

#define FONT_SIZE 28

class Button {
	public:
	
		/**
		 * @param x - x coordinate of button
		 * @param y - y coordinate of button
		 * @param w - width of the button
		 * @param h - height of the button
		 * @param active - true if button should receive input, false if not
		 */
		Button(int x, int y, int w, int h, bool active);
		
		/**
		 * @brief check if button is pressed and trigger its function
		 *
		 * @param mouse_x - current x coordinate of mouse posistion
		 * @param mouse_y - current y coordinate of mouse position
		 * @param left_mouse_button_down - true if LMB is pressed, false otherwise
		 * @param fun - pointer to function that is called when the button is pressed
		 * \todo connect modifier function to button
		 */
		void check_input(float mouse_x, float mouse_y, bool left_mouse_button_down, void (*fun)(int a));
		
		/**
		 * @param renderer - renderer that is used for drawing the button
		 */
		void render(SDL_Renderer* renderer);

	private:
		SDL_FRect button_rect;
		SDL_Color button_color;
		bool previous_state;
		bool active;
		
};

#endif

