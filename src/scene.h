#ifndef _SCENE_H_
#define _SCENE_H_

#include <vector>

#include "button.h"

/**
 * @brief class that contain button and text for 1 screen
 */
class Scene {
	public:
		/**
		 * @param b pointer to button, that will be added to scene
		 */
		void add_button(Button* b);
		
		/**
		 * @brief will add text into the scene
		 *
		 */
		void add_text(Text *t);
		
		/**
		 * @brief same as button check_input
		 */
		void check_input(float mousex, float mousey, bool LMB_state);
		
		/// \cond
		void render(SDL_Renderer* renderer);
	
		void print();
		/// \endcond
		
		/**
		 * @brief clear the scene from all buttons and text
		 */
		void clear();
	private:
		std::vector<Button*> buttons;
		std::vector<Text*> texts;
};

#endif

