#include "../../include/system/EntitiesHandler.h"

#include <algorithm>

std::shared_ptr<EntityHandler> EntitiesHandler::Create_Entity(int Id, const std::string& Name, const Position& Position, const Size& Size, const Color& Color, const SpriteComponent& SpriteComponent)
{
	std::shared_ptr<Entity> entity = std::make_shared<Entity>(Id, Name, Position, Size, Color, SpriteComponent);
	std::shared_ptr<EntityHandler> handler = std::make_shared<EntityHandler>();
	handler->p_Pointer = entity;

	this->entities.push_back(handler);

	return handler;
}

std::shared_ptr<EntityHandler> EntitiesHandler::FindEntityById(int Id)
{
	for (std::shared_ptr<EntityHandler>& handle : this->entities)
	{
		if (handle->p_Pointer && handle->Get_Id() == Id)
		{
			return handle;
		}
	}

	return nullptr;
}

void EntitiesHandler::Delete_Entity(int Id)
{
	for (const std::shared_ptr<EntityHandler>& handler : this->entities)
	{
		if (handler->p_Pointer && handler->p_Pointer->Get_Id() == Id)
		{
			handler->Invalidate();
		}
	}
}
void EntitiesHandler::Cleanup()
{
	this->entities.erase(std::remove_if(entities.begin(), entities.end(), [](const auto& handler) { return !handler->p_Pointer; }), entities.end());
}