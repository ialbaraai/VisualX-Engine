#pragma once

#include <iostream>

#include <vector>
#include <unordered_set>
#include <string>
#include <random>

#include <ctype.h>

#include "Backend.h"
#include "EntityComponentSystem.h"
#include "EntitiesHandler.h"
#include "VXLua.h"

class Game
{
private:
	WindowSize* p_WindowSize = nullptr;
	Color* p_WindowColor = nullptr;
	SDL_Window* p_Window;
	SDL_Renderer* p_Renderer;

	float p_MouseX;
	float p_MouseY;
public:
	std::string p_GameName;
	std::string p_WindowTitle;

	double p_FPS;
	double p_FrameDuration;

	float p_RunTime;

	bool p_IsMouseLeftHeld = false;
	bool p_IsMouseRightHeld = false;
	bool p_IsMouseMiddleHeld = false;

	bool p_IsMouseLeftDown = false;
	bool p_IsMouseLeftUp = false;

	bool p_IsMouseRightDown = false;
	bool p_IsMouseRightUp = false;

	bool p_IsMouseMiddleDown = false;
	bool p_IsMouseMiddleUp = false;

	std::vector<Entity*> p_Entities = {};
	EntitiesHandler p_EntitiesHandler;

	std::unordered_set<SDL_Keycode> p_PreviousKeys = {};
	std::unordered_set<SDL_Keycode> p_CurrentKeys = {};

	bool p_IsRunning;

	VX::LuaManager* p_LuaManager = nullptr;

	std::string p_NextFile;

	Game(const std::string& WindowTitle, const WindowSize& size, const Color& color, const std::vector<Entity*>& entities, const std::string& script);
	~Game();

	// Entity* FindEntityById(int id);
	void HandleInput(const SDL_Event& event);

	void HandleRender();
};

Game* Game_Init(const std::string& WindowTitle, const WindowSize& size, const Color& color, const std::vector<Entity*>& entities, const std::string& script);
void GameLoop(Game* game);

RunResult EndGame(Game* game);