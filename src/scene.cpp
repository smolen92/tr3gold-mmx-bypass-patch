#include "scene.h"

void Scene::add_button(Button* b) {
	buttons.push_back(b);
}

void Scene::add_text(Text* t) {
	texts.push_back(t);
}

void Scene::check_input(float mousex, float mousey, bool LMB_state) {
	for(unsigned int i=0; i < buttons.size(); i++) {
		buttons[i]->check_input(mousex, mousey, LMB_state, NULL);
	}

}

/// \cond
void Scene::render(SDL_Renderer* renderer) {
	for(unsigned int i=0; i < buttons.size(); i++) {
		buttons[i]->render(renderer);
	}

	for(unsigned int i=0; i < texts.size(); i++) {
		texts[i]->render(renderer);
	}
}
/// \endcond


Scene::~Scene() {
	while(!buttons.empty()) {
		delete buttons.back();
		buttons.pop_back();
	}

	while(!texts.empty()) {
		delete texts.back();
		texts.pop_back();
	}
}
