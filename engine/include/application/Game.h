#pragma once

#include <iostream>

#include <vector>
#include <unordered_set>
#include <string>
#include <random>

#include <ctype.h>

#include "../system/Backend.h"
#include "../system/EntityComponentSystem.h"
#include "../system/EntitiesHandler.h"
#include "../utilities/VXLua.h"

namespace VX
{
	class Game
	{
	private:
		WindowSize p_WindowSize = WindowSize();
		Color p_WindowColor = Color();
		SDL_Window* p_Window;
		SDL_Renderer* p_Renderer;

		float p_MouseX;
		float p_MouseY;

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

		std::vector<Entity> p_Entities = {};
		EntitiesHandler p_EntitiesHandler;

		std::unordered_set<SDL_Keycode> p_PreviousKeys = {};
		std::unordered_set<SDL_Keycode> p_CurrentKeys = {};

		bool p_IsRunning;

		VX::LuaManager p_LuaManager;
	public:
		Game();
		~Game();

		void HandleInput(const SDL_Event& event);

		void HandleRender();

		void loop(const std::string& WindowTitle, const WindowSize& size, const Color& color, std::vector<Entity>& entities, const std::string& script);
		RunResult end();
	};
}