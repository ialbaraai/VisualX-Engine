#include "../../include/system/EntityHandler.h"

int EntityHandler::Get_Id() const
{
	return this->p_Pointer ? this->p_Pointer->Get_Id() : -1;
}
bool EntityHandler::Get_IsAlive() const
{
	return this->p_Pointer ? this->p_Pointer->Get_IsAlive() : false;
}
std::string EntityHandler::Get_Name() const
{
	return this->p_Pointer ? this->p_Pointer->Get_Name() : "";
}
Position EntityHandler::Get_Position() const
{
	return this->p_Pointer ? this->p_Pointer->Get_Position() : Position(-1, -1);
}
Size EntityHandler::Get_Size() const
{
	return this->p_Pointer ? this->p_Pointer->Get_Size() : Size(-1, -1);
}
Color EntityHandler::Get_Color() const
{
	return this->p_Pointer ? this->p_Pointer->Get_Color() : Color(-1, -1, -1, -1);
}
SpriteComponent EntityHandler::Get_Sprite() const
{
	return this->p_Pointer ? this->p_Pointer->Get_Sprite() : SpriteComponent(false, "");
}

void EntityHandler::Set_IsAlive(const bool& is_alive)
{
	if (this->p_Pointer)
	{
		this->p_Pointer->Set_IsAlive(is_alive);
	}
}
void EntityHandler::Set_Name(const std::string& name)
{
	if (this->p_Pointer)
	{
		this->p_Pointer->Set_Name(name.c_str());
	}
}
void EntityHandler::Set_Position(const Position& position)
{
	if (this->p_Pointer)
	{
		this->p_Pointer->Set_Position(position);
	}
}
void EntityHandler::Set_Size(const Size& size)
{
	if (this->p_Pointer)
	{
		this->p_Pointer->Set_Size(size);
	}
}
void EntityHandler::Set_Color(const Color& color)
{
	if (this->p_Pointer)
	{
		this->p_Pointer->Set_Color(color);
	}
}
void EntityHandler::Set_Sprite(const SpriteComponent& component)
{
	if (this->p_Pointer)
	{
		this->p_Pointer->Set_Sprite(component);
	}
}

void EntityHandler::Kill()
{
	if (this->p_Pointer)
	{
		this->p_Pointer->Set_IsAlive(false);
	}
}
void EntityHandler::Invalidate()
{
	this->p_Pointer.reset();
}