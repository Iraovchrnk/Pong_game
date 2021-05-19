#include <iostream>
#include "Game.h"
#include "Components.h"
#include "SDL_ttf.h"

const int WIDTH = 10;
const int BALL_HEIGHT = 10;
const int RACKET_HEIGHT = 48;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

Manager manager;
SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

auto &racketPl(manager.addEntity());
auto &racketEn(manager.addEntity());
auto &ball(manager.addEntity());
auto &continueText(manager.addEntity());
auto &scoreText(manager.addEntity());

void Game::init(const char * title, int xpos, int ypos, int width, int height){
	
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		window = SDL_CreateWindow(title, xpos, ypos, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
		if (!window) {
			isRunning = false;
			std::cout << "Failed to create window\n";
			return;
		}

		renderer = SDL_CreateRenderer(window, -1, 0);
		if (!renderer) {
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			isRunning = false;
			std::cout << "Failed to create window\n";
			return;
		}
		
		if (TTF_Init() == -1){
			isRunning = false;
			printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
			return;
		}

		isRunning = true;
		isContinue = false;
	}
	
	initComponents();
}

void Game::Run() {
	const int FPS = 60;
	const int frameDelay = 1000 / FPS;
	Uint32 frameStart;
	int frameTime;

	init("Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT);

	while (isRunning) {
		frameStart = SDL_GetTicks();

		this->handleEvents();
		this->update();
		this->render();

		frameTime = SDL_GetTicks() - frameStart;
		if (frameDelay > frameTime) {
			SDL_Delay(frameDelay - frameTime);
		}
	}

	this->clean();
}


void Game::update() {
	manager.refresh();
	manager.update();

	handleCollision();
	checkScore();
}

void Game::render() {
	SDL_RenderClear(renderer);
	drawBorder();
	manager.draw();
	SDL_RenderPresent(renderer);
}

void Game::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}


void Game::handleEvents(){
	SDL_PollEvent(&event);
	
	if(event.type == SDL_QUIT){
		isRunning = false;
	}
	else if (event.type == SDL_KEYDOWN)	{
		switch (event.key.keysym.sym){
		case SDLK_SPACE:
			if (!isEndGame) {
				contGame();
				isContinue = false;
			}
			else {
				restartGame();
			}
			break;
		case SDLK_q:
			reset();
			break;
		case SDLK_r:
			if (isContinue) {
				restartGame();
			}
			break;
		case SDLK_ESCAPE:
			if (!isContinue && !isEndGame) {
				isContinue = true;
				pauseGame("Continue?");
			}
			else isRunning = false;
			SDL_Delay(200);
			break;
		default:
			break;
		}
	}
}


std::string Game::countScore(){
	std::string score = std::to_string(plScore) + " : " + std::to_string(enScore);
	return score;
}


void Game::checkScore() {
	if (plScore == 15 || enScore == 15) {
		std::string message;
		if (plScore == 15) {
			message = "You Win!";
			
		}
		else if (enScore == 15) {
			message = "You Lose!";
		}
		isEndGame = true;
		pauseGame(message.c_str());
	}
}

void Game::handleCollision(){
	//  Player's acket hit ball
	if(ball.getComponent<ColliderComponent>().collider.x <= racketPl.getComponent<ColliderComponent>().collider.x + WIDTH &&
		ball.getComponent<ColliderComponent>().collider.x >= racketPl.getComponent<ColliderComponent>().collider.x &&
		ball.getComponent<ColliderComponent>().collider.y <= racketPl.getComponent<ColliderComponent>().collider.y + RACKET_HEIGHT &&
		ball.getComponent<ColliderComponent>().collider.y >= racketPl.getComponent<ColliderComponent>().collider.y){
		
		float angle = racketPl.getComponent<KeyboardController>().getMovement() * 0.2;
		ball.getComponent<TransformComponent>().velocity.x *= -1 + angle;
		ball.getComponent<TransformComponent>().pos.x = racketPl.getComponent<TransformComponent>().pos.x + WIDTH;
		ball.getComponent<TransformComponent>().speedIncr();

	}
	// Enemy's racket hit ball
	if (ball.getComponent<ColliderComponent>().collider.x + WIDTH >= racketEn.getComponent<ColliderComponent>().collider.x &&
		ball.getComponent<ColliderComponent>().collider.x + WIDTH <= racketEn.getComponent<ColliderComponent>().collider.x + WIDTH &&
		ball.getComponent<ColliderComponent>().collider.y <= racketEn.getComponent<ColliderComponent>().collider.y + RACKET_HEIGHT &&
		ball.getComponent<ColliderComponent>().collider.y >= racketEn.getComponent<ColliderComponent>().collider.y) {
		
		float angle = racketEn.getComponent<BotComponent>().getMovement() * 0.2;
		ball.getComponent<TransformComponent>().velocity.x *= -1 +angle;
		ball.getComponent<TransformComponent>().pos.x = racketEn.getComponent<TransformComponent>().pos.x - WIDTH;
		ball.getComponent<TransformComponent>().speedIncr();
	}
	//  The Ball came out from the right side 
	if (ball.getComponent<ColliderComponent>().collider.x + WIDTH > SCREEN_WIDTH) {
		ball.getComponent<TransformComponent>().velocity.x *= -1;
		plScore++;
		scoreText.getComponent<TextComponent>().setText(countScore().c_str());
		reset();
		SDL_Delay(100);
	}
	//  The Ball came out from the left side 
	if (ball.getComponent<ColliderComponent>().collider.x <= 0) {
		ball.getComponent<TransformComponent>().velocity.x *= -1;
		enScore++;
		scoreText.getComponent<TextComponent>().setText(countScore().c_str());
		reset();
		SDL_Delay(100);
	}
	// The Ball hit up or down side of the screen
	if (ball.getComponent<ColliderComponent>().collider.y + WIDTH >= SCREEN_HEIGHT ||
		ball.getComponent<ColliderComponent>().collider.y <= 0) {
		ball.getComponent<TransformComponent>().velocity.y *= -1;
	}
}

void Game::initComponents(){
	racketPl.addComponent<TransformComponent>(75, SCREEN_HEIGHT / 2 - WIDTH / 2, WIDTH, RACKET_HEIGHT);
	racketPl.addComponent<SpriteComponent>("assets/racket.bmp");
	racketPl.addComponent<KeyboardController>();
	racketPl.addComponent<ColliderComponent>("racket");

	ball.addComponent<TransformComponent>(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, WIDTH, BALL_HEIGHT);
	ball.addComponent<SpriteComponent>("assets/ball.bmp");
	ball.addComponent<ColliderComponent>("ball");
	ball.getComponent<TransformComponent>().setRandVelocity();

	racketEn.addComponent<TransformComponent>(SCREEN_WIDTH - 75 - WIDTH, SCREEN_HEIGHT / 2 - RACKET_HEIGHT / 2, WIDTH, RACKET_HEIGHT);
	racketEn.addComponent<SpriteComponent>("assets/racket.bmp");
	racketEn.addComponent<ColliderComponent>("racketEn");
	racketPl.addComponent<KeyboardController>();
	racketEn.addComponent<BotComponent>();
	racketEn.getComponent<BotComponent>().setTaregt(&ball);

	continueText.addComponent<TransformComponent>(SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 - 50, 300, 100);
	continueText.addComponent<TextComponent>();

	scoreText.addComponent<TransformComponent>(SCREEN_WIDTH / 2 - 50, 20, 100, 50);
	scoreText.addComponent<TextComponent>(countScore().c_str());

	pauseGame("Press Space to start");
}

void Game::pauseGame(const char* message){
	ball.getComponent<TransformComponent>().setZeroSpeed();
	racketPl.getComponent<TransformComponent>().setZeroSpeed();
	continueText.getComponent<TextComponent>().setText(message);
}

void Game::contGame(){
	ball.getComponent<TransformComponent>().setPrevSpeed();
	racketPl.getComponent<TransformComponent>().setDefaultSpeed();
	continueText.getComponent<TextComponent>().setText("");
}

void Game::restartGame(){
	reset();

	racketPl.getComponent<TransformComponent>().setDefaultSpeed();
	racketPl.getComponent<TransformComponent>().setPos(75, SCREEN_HEIGHT / 2 - BALL_HEIGHT / 2);

	racketEn.getComponent<TransformComponent>().setPos(SCREEN_WIDTH - 75 - WIDTH, SCREEN_HEIGHT / 2 - BALL_HEIGHT / 2);

	plScore = enScore = 0;
	isEndGame = false;
	isContinue = false;
	scoreText.getComponent<TextComponent>().setText(countScore().c_str());
	continueText.getComponent<TextComponent>().setText("");
}

void Game::reset() {
	ball.getComponent <TransformComponent>().setPos(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	ball.getComponent<TransformComponent>().setRandVelocity();
	ball.getComponent<TransformComponent>().setDefaultSpeed();
}


void Game::drawBorder() {
	int x0 = SCREEN_WIDTH / 2;
	int x1 = x0;
	int y0 = 0;
	int y1 = SCREEN_HEIGHT;

	int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
	int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
	int err = dx + dy, e2;
	int count = 0;
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	while (1) {
		if (count < 10) { SDL_RenderDrawPoint(renderer, x0, y0); }
		if (x0 == x1 && y0 == y1) break;
		e2 = 2 * err;
		if (e2 > dy) { err += dy; x0 += sx; }
		if (e2 < dx) { err += dx; y0 += sy; }
		count = (count + 1) % 20;
	}
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}


