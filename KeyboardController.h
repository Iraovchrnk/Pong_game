#pragma once

#include "Game.h"
#include "EntityComponentSystem.h"
#include "Components.h"

enum Movement{STILL = 0, UP = 1, DOWN = -1};

class KeyboardController : public Component {
public:
	TransformComponent *transform;
	Movement lastMove = STILL;

	void init() override {
		transform = &entity->getComponent<TransformComponent>();
	}

	void update() override {
		if (Game::event.type == SDL_KEYDOWN) {
			switch (Game::event.key.keysym.sym) {
			case SDLK_w:
				transform->velocity.y = -1;
				lastMove = UP;
				break;
			case SDLK_s:
				transform->velocity.y = 1;
				lastMove = DOWN;
				break;
			}
		}

		if (Game::event.type == SDL_KEYUP) {
			switch (Game::event.key.keysym.sym) {
			case SDLK_w:
				transform->velocity.y = 0;
				lastMove = STILL;
				break;
			case SDLK_s:
				transform->velocity.y = 0;
				lastMove = STILL;
				break;
			}
		}
	}

	Movement getMovement() {
		return lastMove;
	}
};
