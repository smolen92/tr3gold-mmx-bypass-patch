#ifndef _SCENE_H_
#define _SCENE_H_

#include <vector>

#include <SDL3/SDL.h>

#include "button.h"
#include "text.h"

class Scene {
	public:
		/**
		 * @param b pointer to button, that will be added to scene
		 */
		void add_button(Button* b);
		
		/**
		 * @brief will add text into the scene
		 *
		 * @param t text itself
		 * @param c color of the text
		 */
		void add_text(const char* t, SDL_Color c);
		
		/**
		 * @brief same as button check_input
		 */
		void check_input(float mousex, float mousey, bool LMB_state);
		
		/**
		 * @param renderer renderer that will be used for rendering
		 * @param font font for the text
		 */
		void render(SDL_Renderer* renderer, TTF_Font* font);
		
		~Scene();
	private:
		std::vector<Button*> buttons;
		std::vector<Text*> texts;
};

#endif

