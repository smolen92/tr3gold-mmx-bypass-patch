#ifndef _GUI_H_
#define _GUI_H_

#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "scene.h"

/// \cond
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 200
/// \endcond

/**
 * @brief class for handling gui using sdl
 */
class Gui {
	public:
		/**
		 * @param font poitner to font, that will be used for text
		 */
		int gui_init(TTF_Font** font);

		/**
		 * @brief checks the input
		 *
		 * @param running boolean that will tell if the main loop should run
		 */
		void input(bool *running);
		
		/**
		 * @brief do the logic only for gui
		 */
		void logic();

		/**
		 * @brief render the scene to window
		 */
		void render();
		
		/**
		 * @brief print the scene to terminal/cli
		 */
		void print();

		~Gui();
			
		/// \cond
		Scene* current_scene;
		SDL_Renderer* renderer;
		/// \endcond
	private:
		SDL_Window* window;
		float mouse_x, mouse_y;
		bool left_mouse_button_down;
};

#endif

