#pragma once

#include <iostream>
#include <string>

#include <sol/sol.hpp>

namespace VX
{
	class LuaManager
	{
	private:
		std::string File;
	public:
		sol::state LuaState;
		LuaManager(const std::string& File);
		bool LoadScript();
	};
};

VX::LuaManager* VXLua_Init(const std::string& LuaFile);
void VXLua_Destroy(VX::LuaManager*& Manager);