#pragma once

#include <iostream>
#include "Entity.h"
#include <memory>

class EntityHandler
{
public:
	std::shared_ptr<Entity> p_Pointer;

	int Get_Id() const;
	bool Get_IsAlive() const;
	std::string Get_Name() const;
	Position Get_Position() const;
	Size Get_Size() const;
	Color Get_Color() const;
	SpriteComponent Get_Sprite() const;

	void Set_IsAlive(const bool& is_alive);
	void Set_Name(const std::string& name);
	void Set_Position(const Position& position);
	void Set_Size(const Size& size);
	void Set_Color(const Color& color);
	void Set_Sprite(const SpriteComponent& component);

	void Kill();
	void Invalidate();
};