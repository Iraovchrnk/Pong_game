#pragma once

#include "Components.h"
#include "EntityComponentSystem.h"
#include "TextureManager.h"

class SpriteComponent : public Component {
private:
	TransformComponent *transform;
	SDL_Texture *texture;
	SDL_Rect srcRect, destRect;
public:
	SpriteComponent() = default;
	SpriteComponent(const char* path) {
		SetTex(path);
	}

	~SpriteComponent() {
		SDL_DestroyTexture(texture);
	}

	void SetTex(const char* path) {
		texture = TextureManager::LoadTexture(path);
	}

	void init() override {

		transform = &entity->getComponent<TransformComponent>();

		srcRect.x = srcRect.y = 0;
		srcRect.w = transform->width;
		srcRect.h = transform->height;

	}

	void draw() override {
		TextureManager::Draw(texture, srcRect, destRect);
	}

	void update() override {
		destRect.x = static_cast<int>(transform->pos.x);
		destRect.y = static_cast<int>(transform->pos.y);
		destRect.h = transform->height;
		destRect.w = transform->width;
	}
};
