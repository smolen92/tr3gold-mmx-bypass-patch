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


class Gui {
	public:
		int gui_init(TTF_Font** font);

		void input(bool *running);
		
		void logic();

		void render();
		
		void print();

		~Gui();

		Scene* current_scene;
	private:
		SDL_Renderer* renderer;
		SDL_Window* window;
		float mouse_x, mouse_y;
		bool left_mouse_button_down;
};

#endif

