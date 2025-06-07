#pragma once

#include "Vec2.h"
#include <string>

class Action
{
public:
	Action();
	Action(const std::string& name, const std::string& type);
	Action(const std::string& name, Vec2 pos);
	Action(const std::string& name, const std::string& type, Vec2 pos);

	const std::string& name() const;
	const std::string& type() const;
	const Vec2& pos() const;
	std::string toString() const;

private:
	std::string m_name = "None";
	std::string m_type = "None";
	Vec2 m_pos = Vec2(0, 0);
};
