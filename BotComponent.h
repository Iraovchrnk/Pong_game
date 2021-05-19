#pragma once
#include "KeyboardController.h"

class BotComponent : public Component {
public:
	TransformComponent *transform;
	TransformComponent *target;
	Movement lastMove = STILL;
	BotComponent() {}


	void setTaregt(Entity* e) {
		this->target = &e->getComponent<TransformComponent>();
	}

	void init() override {
		transform = &entity->getComponent<TransformComponent>();
	}

	void update() override {
		if (transform->pos.y + transform->height / 2 >= target->pos.y + target->height) {
			transform->pos.y -= transform->speed;
			lastMove = UP;
		}
		else if (transform->pos.y + transform->height / 2 <= target->pos.y) {
			transform->pos.y += transform->speed;
			lastMove = DOWN;
		}
		else {
			lastMove = STILL;
		}
	}

	Movement getMovement() {
		return lastMove;
	}
};



