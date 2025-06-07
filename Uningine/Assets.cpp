#include "Assets.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include <vector>
Assets::Assets()
{
}

void Assets::loadFromFile(const std::string& path)
{
	std::ifstream file(path);
	if (!file)
	{
		std::cerr << "Error: could not load asset file " << path << std::endl;
		return;
	}
	
	std::string line, token, identifier;
	std::vector<std::string> tempVector;

	while (std::getline(file, line))
	{
		std::istringstream lineStream(line);
		while (lineStream >> token)
		{
			if (token == "Texture" || token == "Animation" || token == "Font")
			{
				identifier = token;
				tempVector.clear();
				continue;
			}
			tempVector.push_back(token);
		}

		if (identifier == "Texture" && tempVector.size() >= 2)
		{
			bool repeated = false;
			if (tempVector.size() == 3)
			{
				std::istringstream iss(tempVector[2]);
				iss >> repeated;
			}
			addTexture(tempVector[0], tempVector[1], repeated);
		}

		else if (identifier == "Animation" && tempVector.size() == 4)
		{
			std::istringstream iss1(tempVector[2]);
			std::istringstream iss2(tempVector[3]);
			size_t frameCount, interval;
			iss1 >> frameCount;
			iss2 >> interval;
			addAnimation(tempVector[0], tempVector[1], frameCount, interval);
			iss1.clear();
			iss2.clear();
		}

		else if (identifier == "Font" && tempVector.size() == 2)
		{
			addFont(tempVector[0], tempVector[1]);
		}
	}
}


const std::map<std::string, sf::Texture>& Assets::getTextures() const
{
	return m_textureMap;
}

const std::map<std::string, Animation>& Assets::getAnimations() const
{
	return m_animationMap;
}

const std::map<std::string, sf::Sound>& Assets::getSounds() const
{
	return m_soundMap;
}

const std::map<std::string, std::string>& Assets::getMusic() const
{
	return m_musicMap;
}

const sf::Texture& Assets::getTexture(const std::string& textureName) const
{
	assert(m_textureMap.find(textureName) != m_textureMap.end());
	return m_textureMap.at(textureName);
}

const Animation& Assets::getAnimation(const std::string& animationName) const
{
	assert(m_animationMap.find(animationName) != m_animationMap.end());
	return m_animationMap.at(animationName);
}

const sf::Font& Assets::getFont(const std::string& fontName) const
{
	assert(m_fontMap.find(fontName) != m_fontMap.end());
	return m_fontMap.at(fontName);
}

sf::Sound& Assets::getSound(const std::string& soundName)
{
	assert(m_soundMap.find(soundName) != m_soundMap.end());
	return m_soundMap.at(soundName);
}

const std::string& Assets::getMusic(const std::string& musicName) const
{
	assert(m_musicMap.find(musicName) != m_musicMap.end());
	return m_musicMap.at(musicName);
}

void Assets::addTexture(const std::string& textureName, const std::string& path, bool repeated)
{
	sf::Texture texture;
	if (!texture.loadFromFile(path))
	{
		std::cerr << "Error: could not load texture from path: " << path << std::endl;
		return;
	}

	texture.setSmooth(false);
	texture.setRepeated(repeated);
	m_textureMap[textureName] = texture;
}

void Assets::addAnimation(const std::string& animationName, const std::string& textureName, size_t frameCount, size_t speed)
{
	m_animationMap[animationName] = Animation(animationName, getTexture(textureName), frameCount, speed);
}

void Assets::addFont(const std::string& fontName, const std::string& path)
{
	sf::Font font;
	if (!font.loadFromFile(path))
	{
		std::cerr << "Error: could not load font " << path << std::endl;
		return;
	}
	m_fontMap[fontName] = font;
}

void Assets::addSound(const std::string& soundName, const std::string& path)
{
	sf::SoundBuffer buffer;
	if (!buffer.loadFromFile(path))
	{
		std::cerr << "Error: could not load sound " << path << std::endl;
		return;
	}
	m_soundBufferMap[soundName] = buffer;
	m_soundMap[soundName] = sf::Sound(buffer);
}

void Assets::addMusic(const std::string& musicName, const std::string& path)
{
	m_musicMap[musicName] = path;
}
