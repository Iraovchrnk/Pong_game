#pragma once
#include <string>
#include "SDL.h"
#include "Components.h"

class ColliderComponent : public Component {
	
public:
	SDL_Rect collider;
	std::string tag;

	TransformComponent* transform;

	ColliderComponent(std::string t) {
		tag = t;
	}

	void init() override {
		if (!entity->hasComponent<TransformComponent>()) {
			entity->addComponent<TransformComponent>();
		}
		transform = &entity->getComponent<TransformComponent>();
	}

	void update() override {
 		collider.x = static_cast<int>(transform->pos.x);
		collider.y = static_cast<int>(transform->pos.y);
		collider.h = transform->height;
		collider.w = transform->width;
	}

};

