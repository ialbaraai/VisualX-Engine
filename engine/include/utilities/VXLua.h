#pragma once

#include <iostream>
#include <string>

#include <sol/sol.hpp>

namespace VX
{
	class LuaManager
	{
	public:
		std::string File;
		sol::state LuaState;

		LuaManager();
		LuaManager(const char* luafile);

		bool LoadScript();
	};
};