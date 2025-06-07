#include "Physics.h"
#include "SFML/Graphics.hpp"
#include <iostream>


Vec2 Physics::GetOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{
	if (a->has<CBoundingBox>() && b->has<CBoundingBox>())
	{
		Vec2 delta(abs(a->get<CTransform>().pos.x - b->get<CTransform>().pos.x), abs(a->get<CTransform>().pos.y - b->get<CTransform>().pos.y));
		float OverlapX = a->get<CBoundingBox>().halfSize.x + b->get<CBoundingBox>().halfSize.x - delta.x;
		float OverlapY = a->get<CBoundingBox>().halfSize.y + b->get<CBoundingBox>().halfSize.y - delta.y;
		return Vec2(OverlapX, OverlapY);
	}
	else
	{
		return Vec2(0, 0);
	}
}

Vec2 Physics::GetPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{
	if (a->has<CBoundingBox>() && b->has<CBoundingBox>())
	{
		Vec2 delta(abs(a->get<CTransform>().prevPos.x - b->get<CTransform>().prevPos.x), abs(a->get<CTransform>().prevPos.y - b->get<CTransform>().prevPos.y));
		float OverlapX = a->get<CBoundingBox>().halfSize.x + b->get<CBoundingBox>().halfSize.x - delta.x;
		float OverlapY = a->get<CBoundingBox>().halfSize.y + b->get<CBoundingBox>().halfSize.x - delta.y;
		return Vec2(OverlapX, OverlapY);
	}
	else
	{
		return Vec2(0, 0);
	}
}

bool Physics::IsInside(const Vec2& pos, std::shared_ptr<Entity> e)
{
	return false;
}

Intersect Physics::LineIntersect(const Vec2& a, const Vec2& b, const Vec2& c, const Vec2& d)
{
	Vec2 r = b - a;
	Vec2 s = d - c;
	float rxs = r.cross(s);
	Vec2 cma = c - a;
	float t = cma.cross(s) / rxs;
	float u = cma.cross(r) / rxs;

	if (t >= 0 && t <= 1 && u >= 0 && u <= 1)
	{
		return { true, Vec2(a.x + t * r.x, a.y + t * r.y) };
	}
	else
	{
		return { false, Vec2(0,0) };
	}
}

bool Physics::EntityIntersect(const Vec2& a, const Vec2& b, std::shared_ptr<Entity> e)
{
	sf::FloatRect globalBounds = e->get<CAnimation>().animation.getSprite().getGlobalBounds();
	Vec2 topLeft = Vec2(globalBounds.left, globalBounds.top);
	Vec2 topRight = Vec2(globalBounds.left + globalBounds.width, globalBounds.top);
	Vec2 bottomLeft = Vec2(globalBounds.left, globalBounds.top + globalBounds.height);
	Vec2 bottomRight = Vec2(globalBounds.left + globalBounds.width, globalBounds.top + globalBounds.height);

	if (LineIntersect(a, b, topLeft, topRight).intersected)
	{
		return true;
	}
	else if (LineIntersect(a, b, topRight, bottomRight).intersected)
	{
		return true;
	}
	else if (LineIntersect(a, b, bottomRight, bottomLeft).intersected)
	{
		return true;
	}
	else if (LineIntersect(a, b, bottomLeft, topLeft).intersected)
	{
		return true;
	}

	return false;
}

void Physics::ResolveCollision(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{
	Vec2 overlap = GetOverlap(a, b);

	if (overlap.x > 0 && overlap.y > 0)
	{
		Vec2 pushAmount(0.0f, 0.0f);

		bool isAStatic = a->has<CStatic>();
		bool isBStatic = b->has<CStatic>();

		if (isAStatic && isBStatic)
		{
			return;
		}

		else if (isAStatic != isBStatic)
		{
			auto& movingEntity = isAStatic ? b : a;
			auto& staticEntity = isAStatic ? a : b;

			Vec2 mPos = movingEntity->get<CTransform>().pos;
			Vec2 sPos = staticEntity->get<CTransform>().pos;

			if (overlap.x < overlap.y)
			{
				float dir = (mPos.x > sPos.x) ? 1.0f : -1.0f;
				pushAmount.x = dir * overlap.x;
			}
			else
			{
				float dir = (mPos.y > sPos.y) ? 1.0f : -1.0f;
				pushAmount.y = dir * overlap.y;
			}
			movingEntity->get<CTransform>().pos -= pushAmount;
		}

		else
		{
			Vec2 pushDirection = (a->get<CTransform>().pos - b->get<CTransform>().pos);
			pushDirection.normalize();
			pushAmount = Vec2(pushDirection.x * overlap.x, pushDirection.y * overlap.y) / 2.0f;
			a->get<CTransform>().pos += pushAmount;
			b->get<CTransform>().pos -= pushAmount;
		}
	}
}
