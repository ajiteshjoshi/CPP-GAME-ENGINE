#include "Animation.h"
#include <iostream>

Animation::Animation()
{
}

Animation::Animation(const std::string& name, const sf::Texture& t)
	: Animation(name, t, 1, 0)
{
}

Animation::Animation(const std::string& name, const sf::Texture& t, size_t frameCount, size_t speed)
	: m_name(name), m_sprite(t), m_frameCount(frameCount), m_currentFrame(0), m_speed(speed)
{
	m_size = Vec2((float)t.getSize().x / frameCount, (float)t.getSize().y);
	m_sprite.setOrigin(m_size.x / 2.0f, m_size.y / 2.0f);
	m_sprite.setTextureRect(sf::IntRect((int)std::floor(m_currentFrame) * (int)m_size.x, 0, (int)m_size.x, (int)m_size.y));
}

void Animation::update()
{
	if (m_speed)
	{
		m_currentFrame++;
		size_t animFrame = (m_currentFrame * 8 / m_speed) % m_frameCount;
		m_sprite.setTextureRect(sf::IntRect((int)std::floor(animFrame) * (int)m_size.x, 0, (int)m_size.x, (int)m_size.y));
	}
}

bool Animation::hasEnded() const
{
	return false;
}

const std::string& Animation::getName() const
{
	return m_name;
}

const Vec2& Animation::getSize() const
{
	return m_size;
}

sf::Sprite& Animation::getSprite()
{
	return m_sprite;
}

const sf::Sprite& Animation::getSprite() const
{
	return m_sprite;
}
