#pragma once
#include "Components.h"
#include "EntityComponentSystem.h"
#include "TextureManager.h"

class TextComponent : public Component {
private:
	TransformComponent *transform;
	SDL_Texture *texture;

	TTF_Font *font = TTF_OpenFont("assets/Lato-Black.ttf", 24);
	SDL_Rect srcRect, destRect;
public:
	TextComponent() = default;
	TextComponent(const char* text) {
		setText(text);
	}

	~TextComponent() {
		SDL_DestroyTexture(texture);
		TTF_CloseFont(font);
	}

	void setText(const char* text = "") {
		texture = TextureManager::LoadText(text, font);
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