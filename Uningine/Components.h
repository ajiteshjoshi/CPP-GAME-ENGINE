#pragma once

#include "Vec2.h"
#include "Animation.h"
#include "SFML/Graphics.hpp"
#include <string>

class Component
{
public:
	bool has = false;
};

class CTransform : public Component
{
public:
	Vec2 pos = { 0.0, 0.0 };
	Vec2 prevPos = { 0.0, 0.0 };
	Vec2 scale = { 1.0, 1.0 };
	Vec2 velocity = { 0.0, 0.0 };
	Vec2 facing = { 0.0, 1.0 };
	float angle = 0;

	CTransform()
	{
	}
	CTransform(const Vec2& p)
		: pos(p) 
	{
	}
	CTransform(const Vec2& p, const Vec2& sp, const Vec2& sc, float a)
		: pos(p), prevPos(p), velocity(sp), scale(sc), angle(a) 
	{
	}
};

class CShape : public Component
{
public:
	sf::CircleShape Shape;

	CShape()
	{
	}
	CShape(const float  radius, const size_t pointCount, const sf::Color& fillColor, const sf::Color& outlineColor, const float thickness) {
		Shape = sf::CircleShape(radius, pointCount);
		Shape.setFillColor(fillColor);
		Shape.setOutlineColor(outlineColor);
		Shape.setOutlineThickness(thickness);
		Shape.setOrigin(radius, radius);
	}
};

class CLifespan : public Component
{
public:
	int lifespan = 0;
	int frameCreated = 0;
	CLifespan()
	{
	}
	CLifespan(int duration, int frame)
		: lifespan(duration), frameCreated(frame)
	{
	}
};

class CInput : public Component
{
public:
	bool up = false;
	bool down = false;
	bool left = false;
	bool right = false;
	bool attack = false;
	bool canAttack = true;

	CInput()
	{
	}
};

class CBoundingBox : public Component
{
public:
	Vec2 pos;
	Vec2 offset;
	Vec2 size;
	Vec2 halfSize;
	bool blockMove = false;
	bool blockVision = false;
	CBoundingBox()
	{
	}
	CBoundingBox(const Vec2& p, const Vec2& o, const Vec2& s)
		: pos(p), offset(o), size(s), halfSize(s.x / 2, s.y / 2)
	{
	}
	CBoundingBox(const Vec2& p, const Vec2& o, const Vec2& s, bool m, bool v)
		: pos(p), offset(o), size(s), blockMove(m), blockVision(v), halfSize(s.x / 2, s.y / 2)
	{
	}
};

class CAnimation : public Component
{
public:
	Animation animation;
	bool repeat = false;
	CAnimation()
	{
	}
	CAnimation(const Animation& anim, bool r)
		: animation(anim), repeat(r)
	{
	}
};

class CState : public Component
{
public:
	std::string state = "stand";
	CState() {}
	CState(const std::string& s) : state(s) {}
};

class CStatic : public Component
{
public:
	CStatic() {}
};
