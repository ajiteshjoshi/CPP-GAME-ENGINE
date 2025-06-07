#pragma once

#include "Components.h"

#include <tuple>
#include <string>

class EntityManager;

typedef std::tuple <CTransform, CShape, CLifespan, CInput, CBoundingBox, CAnimation, CState, CStatic> ComponentTuple;

class Entity
{
public:
	void Destroy();
	const size_t id() const;
	bool isActive() const;
	const std::string& tag() const;

	template <typename T>
	bool has()
	{
		return get<T>().has;
	}

	template <typename T, typename... TArgs>
	T& add(TArgs&&... mArgs)
	{
		auto& component = get<T>();
		component = T(std::forward<TArgs>(mArgs)...);
		component.has = true;
		return component;
	}

	template <typename T>
	T& get()
	{
		return std::get<T>(m_components);
	}

	template <typename T>
	void remove()
	{
		get<T>();
	}

private:
	friend class EntityManager;

	bool m_active = true;
	std::string m_tag = "default";
	size_t m_id = 0;
	ComponentTuple m_components;

	Entity(const size_t id, const std::string& tag);
};
