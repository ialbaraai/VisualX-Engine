#include "Entity.h"

Entity::Entity(const int& id, const std::string& name, const Position& position, const Size& size, const Color& color, const SpriteComponent& component)
{
	this->p_Id = id;
	this->p_Name = name;

	this->p_Position->X = position.X;
	this->p_Position->Y = position.Y;

	this->p_Size->X = size.X;
	this->p_Size->Y = size.Y;

	this->p_Color->Red = color.Red;
	this->p_Color->Green = color.Green;
	this->p_Color->Blue = color.Blue;
	this->p_Color->Alpha = color.Alpha;

	delete this->p_Sprite;
	this->p_Sprite = new SpriteComponent(component.IsImage, component.FilePath);

	this->p_IsAlive = true;
}
Entity::~Entity()
{
	delete this->p_Position;
	delete this->p_Size;
	delete this->p_Color;
	delete this->p_Sprite;
}