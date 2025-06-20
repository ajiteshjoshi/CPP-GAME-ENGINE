#pragma once

#include "Vec2.h"
#include <vector>
#include <SFML/Graphics.hpp>

class Animation
{
public:
	Animation();
	Animation(const std::string& name, const sf::Texture& t);
	Animation(const std::string& name, const sf::Texture& t, size_t frameCount, size_t speed);

	void update();
	bool hasEnded() const;
	const std::string& getName() const;
	const Vec2& getSize() const;
	sf::Sprite& getSprite();
	const sf::Sprite& getSprite() const;

private:
	sf::Sprite m_sprite;
	size_t m_frameCount = 1;
	size_t m_currentFrame = 0;
	size_t m_gameFrame = 0;
	size_t m_speed = 0;
	Vec2 m_size = { 1,1 };
	std::string m_name = "none";
};