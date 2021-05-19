#pragma once
#include "SDL.h"
#include <iostream>

class ColliderComponent;

class Game {
public:
	static SDL_Renderer* renderer;
	static SDL_Event event;

	void Run();

private:
	SDL_Window *window;

	bool isRunning = false;
	bool isContinue = true;
	bool isEndGame = false;
	void drawBorder();
	int plScore = 0;
	int enScore = 0;

	void init(const char* title, int xpos, int  ypos, int width, int height);
	void handleEvents();
	void update();
	void render();
	void clean();

	std::string countScore();

	void handleCollision();
	void initComponents();
	void reset();
	void pauseGame(const char* message);
	void contGame();
	void restartGame();
	void checkScore();
};


