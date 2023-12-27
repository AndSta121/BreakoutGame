#pragma once

class Vector2 {
public:
	float x, y;
	Vector2();
	Vector2(float x, float y);
	void SetVector(float x, float y);
	Vector2& operator= (const Vector2& vector2);
	Vector2& operator+= (const Vector2& vector2);
	Vector2 operator* (float multiplier);
	Vector2 operator/ (float dividend);
};