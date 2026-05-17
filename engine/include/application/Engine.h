#pragma once

#define SOL_ALL_SAFETIES_ON 1

#include <string>
#include <stdbool.h>

#include "../system/Backend.h"
#include "../system/EntityComponentSystem.h"

namespace VX
{
	class Engine
	{
	private:
		std::string input = "";
		char choice;
		bool is_sdl;
		vxs width;
		vxs height;
		vxm theme;
	public:
		Engine();
		void run();
	};
};