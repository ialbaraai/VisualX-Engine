#pragma once

#include <iostream>
#include <vector>

#include "EntityHandler.h"

class EntitiesHandler
{
public:
	std::vector<std::shared_ptr<EntityHandler>> entities;

	std::shared_ptr<EntityHandler> Create_Entity(int Id, const std::string& Name, Position& Position, Size& Size, Color& Color, SpriteComponent& SpriteComponent);

	std::shared_ptr<EntityHandler> FindEntityById(int Id);

	void Delete_Entity(int Id);
	void Cleanup();
};