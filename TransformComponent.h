#pragma once
#include "Components.h"	
#include "Vector2D.h"
#include <time.h>
#include <random>

struct TransformComponent : public Component {
public:
	float speed = 3;
	float defaultSpeed = 3;
	float currentSpeed = 3;

	int height;
	int width;
	Vector2D pos;
	Vector2D velocity;

	TransformComponent() = default;
	
	TransformComponent(float x, float y, int w, int h) {
		pos.x = x;
		pos.y = y;
		height = h;
		width = w;
	}

	void init() override {
		velocity.Zero();
		srand(static_cast <unsigned> (time(0)));
	}

	void update() override {
		pos.x += velocity.x * speed;
		pos.y += velocity.y * speed;
	}

	float genRandFloat() {
		static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
		float x = static_cast<float>(rand() * fraction * (1) + 0.3);
		int sign = static_cast<int>(rand() * (1));
		float value = (int)(x * 100);
		x = (float)value / 100;
		x *= (sign % 2 == 0 ? 1 : -1);
		//std::cout << sign << ' ' << x << "\n\n";
		return x;
	}

	void setPos(float x, float y) {
		pos.x = x;
		pos.y = y;
	}

	void setRandVelocity() {
		velocity.x = genRandFloat();
		velocity.y = genRandFloat();
	}

	void setZeroSpeed() {
		speed = 0;
	}

	void setDefaultSpeed() {
		currentSpeed = defaultSpeed;
		speed = defaultSpeed;
	}

	void setPrevSpeed() {
		speed = currentSpeed;
	}

	void speedIncr() {
		currentSpeed += 0.3;
		speed = currentSpeed;
	}
};


