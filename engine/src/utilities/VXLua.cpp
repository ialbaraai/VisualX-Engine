#include "../../include/utilities/VXLua.h"

VX::LuaManager::LuaManager()
{
	this->LuaState.open_libraries(sol::lib::base, sol::lib::io, sol::lib::math, sol::lib::table, sol::lib::os);
	this->File = "";
}
VX::LuaManager::LuaManager(const char* luafile)
{
	this->LuaState.open_libraries(sol::lib::base, sol::lib::io, sol::lib::math, sol::lib::table, sol::lib::os);
	this->File = luafile;
}

bool VX::LuaManager::LoadScript()
{
	if (!this->File.empty())
	{
		auto result = this->LuaState.script_file(this->File);

		if (!result.valid())
		{
			sol::error err = result;
			std::cerr << "Lua Error: " << err.what() << "\n";

			return false;
		}

		return true;
	}
	else
	{
		return false;
	}
}