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
		vxs width;
		vxs height;
		vxm theme;

		int selection;

		bool engine_running = false;
	public:
		Engine();
		int run();
	};
};