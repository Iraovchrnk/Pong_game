#include "TextureManager.h"

const SDL_Color White = { 255, 255, 255 };

SDL_Texture * TextureManager::LoadTexture(const char * path){
	SDL_Surface* tmps = SDL_LoadBMP(path);
	SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer, tmps);
	SDL_FreeSurface(tmps);

	return tex;
}

SDL_Texture * TextureManager::LoadText(const char * text, TTF_Font* font) {
	SDL_Surface* tmps = TTF_RenderText_Solid(font, text, White);
	SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer, tmps);
	
	SDL_FreeSurface(tmps);

	return tex;
}

void TextureManager::Draw(SDL_Texture * tex, SDL_Rect src, SDL_Rect dst){
	SDL_RenderCopy(Game::renderer, tex, &src, &dst);
}
