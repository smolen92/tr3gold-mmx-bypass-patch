#ifndef _SCENE_H_
#define _SCENE_H_

#include <vector>

#include <SDL3/SDL.h>

#include "button.h"
#include "text.h"

class Scene {
	public:
		void add_button(Button* b);
		void add_text(const char* t, SDL_Color c);
		void check_input(float mousex, float mousey, bool LMB_state);
		void render(SDL_Renderer* renderer, TTF_Font* font);
		~Scene();
	private:
		std::vector<Button*> buttons;
		std::vector<Text*> texts;
};

#endif

