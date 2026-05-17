#pragma once

#include <iostream>

#include "Backend.h"
#include "EntityComponentSystem.h"

class Entity
{
private:
	int p_Id;
	Position* p_Position        = new Position();
	Size* p_Size                = new Size();
	Color* p_Color              = new Color();
	SpriteComponent* p_Sprite	= new SpriteComponent(false, "");
	std::string p_Name;
	bool p_IsAlive = false;
public:
	Entity(const int& id, const std::string& name, const Position& position, const Size& size, const Color& color, const SpriteComponent& sprite);
	virtual ~Entity();

	const std::string& Get_Name() const
	{
		return this->p_Name;
	}
	void Set_Name(const char* name)
	{
		this->p_Name			= name;
	}

	int Get_Id() const
	{
		return this->p_Id;
	}

	Position& Get_Position() const
	{
		return *this->p_Position;
	}
	void Set_Position(const Position& position)
	{
		this->p_Position->X	    = position.X;
		this->p_Position->Y     = position.Y;
	}

	Size& Get_Size() const
	{
		return *this->p_Size;
	}
	void Set_Size(const Size& size)
	{
		this->p_Size->X		= size.X;
		this->p_Size->Y		= size.Y;
	}

	Color& Get_Color() const
	{
		return *this->p_Color;
	}
	void Set_Color(const Color& color)
	{
		this->p_Color->Red		= color.Red;
		this->p_Color->Green	= color.Green;
		this->p_Color->Blue		= color.Blue;
		this->p_Color->Alpha	= color.Alpha;
	}

	SpriteComponent& Get_Sprite() const
	{
		if (this->p_IsAlive)
			return *this->p_Sprite;
	}
	void Set_Sprite(const SpriteComponent& sprite)
	{
		delete this->p_Sprite;
		this->p_Sprite = new SpriteComponent(sprite.IsImage, sprite.FilePath);
	}

	bool Get_IsAlive() const
	{
		return this->p_IsAlive;
	}
	void Set_IsAlive(const bool& toggle)
	{
		this->p_IsAlive			= toggle;
	}
};
