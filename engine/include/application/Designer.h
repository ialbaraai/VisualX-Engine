#pragma once

#include <string>
#include <vector>

#include "../system/Backend.h"
#include "../system/Entity.h"
#include "../system/EntityComponentSystem.h"

namespace VX
{
	class Designer
	{
	private:
		SDL_Window* p_Window = nullptr;
		SDL_Renderer* p_Renderer = nullptr;
		vxm p_Theme;

		std::string p_Filepath = "";

		std::vector<std::string> p_Data = {};
	public:
		Designer(const std::string& filepath, const vxm theme);
		void run();
		void apply(const WindowSize& windowSize, const Color& windowColor, const std::string& scriptFilepath, std::vector<Entity>& entities);
	};
};