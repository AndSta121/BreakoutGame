#include"Vector2.h"

Vector2::Vector2()
{
	x = 0.0f;
	y = 0.0f;
}

Vector2::Vector2(float x, float y)
{
	this->x = x;
	this->y = y;
}

void Vector2::SetVector(float x, float y)
{
	this->x = x;
	this->y = y;
}

Vector2& Vector2::operator=(const Vector2& vector2)
{
	this->x = vector2.x;
	this->y = vector2.y;

	return *this;
}

Vector2& Vector2::operator+=(const Vector2& vector2)
{
	this->x += vector2.x;
	this->y += vector2.y;

	return *this;
}

Vector2 Vector2::operator * (float multiplier)
{
	return Vector2(this->x*multiplier, this->y*multiplier);
}

Vector2 Vector2::operator/(float dividend)
{
	return Vector2(this->x/dividend, this->y/dividend);
}
