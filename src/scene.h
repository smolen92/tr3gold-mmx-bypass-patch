#ifndef _SCENE_H_
#define _SCENE_H_

#include <vector>

#include "button.h"
#include "text.h"

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
		 * @param t text itself
		 * @param y y position of the text
		 * @param c color of the text
		 * @param f font to use
		 */
		void add_text(Text *t);
		
		/**
		 * @brief same as button check_input
		 */
		void check_input(float mousex, float mousey, bool LMB_state);
		
		/// \cond
		void render(SDL_Renderer* renderer);
		/// \endcond
		
		~Scene();
	private:
		std::vector<Button*> buttons;
		std::vector<Text*> texts;
};

#endif

