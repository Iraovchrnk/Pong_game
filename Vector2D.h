#pragma once
#include <iostream>

class Vector2D {
public:
	float x;
	float y;

	Vector2D();
	Vector2D(float x, float y);

	Vector2D& Add(const Vector2D& vec);
	Vector2D& Substr(const Vector2D& vec);
	Vector2D& Mult(const Vector2D& vec);
	Vector2D& Div(const Vector2D& vec);

	friend Vector2D& operator+(Vector2D& v1, const Vector2D& v2);
	friend Vector2D& operator-(Vector2D& v1, const Vector2D& v2);
	friend Vector2D& operator/(Vector2D& v1, const Vector2D& v2);
	friend Vector2D& operator*(Vector2D& v1, const Vector2D& v2);
	
	Vector2D& operator*(const int i);
	Vector2D& operator-(const int i);
	Vector2D& operator+(const int i);
	Vector2D& Zero();

	Vector2D& operator+=(const Vector2D& v);
	Vector2D& operator-=(const Vector2D& v);
	Vector2D& operator*=(const Vector2D& v);
	Vector2D& operator/=(const Vector2D& v);

	friend std::ostream& operator<<(std::ostream& stream, const Vector2D& vec);

};