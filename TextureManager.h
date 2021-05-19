#pragma once
#include "Game.h"
#include "SDL_ttf.h"

class TextureManager {
	
public:
	static SDL_Texture* LoadTexture(const char* fileName);
	static SDL_Texture * LoadText(const char * text, TTF_Font* font);
	static void Draw(SDL_Texture *tex, SDL_Rect src, SDL_Rect  dst);
};