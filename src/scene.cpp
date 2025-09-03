#include "scene.h"

void Scene::add_button(Button* b) {
	buttons.push_back(b);
}

void Scene::add_text(const char* t, SDL_Color c) {
	texts.push_back(new Text);
	texts.back()->set_text(t, c);
}

void Scene::check_input(float mousex, float mousey, bool LMB_state) {
	for(int i=0; i < buttons.size(); i++) {
		buttons[i]->check_input(mousex, mousey, LMB_state, NULL);
	}

}

void Scene::render(SDL_Renderer* renderer, TTF_Font* font) {
	for(int i=0; i < buttons.size(); i++) {
		buttons[i]->render(renderer);
	}

	for(int i=0; i < texts.size(); i++) {
		texts[i]->render(renderer, font);
	}
}

Scene::~Scene() {
	while(!buttons.empty()) {
		delete buttons.back();
		buttons.pop_back();
	}

	while(!texts.empty()) {
		texts.back()->free_text();
		delete texts.back();
		texts.pop_back();
	}
}
