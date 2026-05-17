#include "VXLua.h"

VX::LuaManager* VXLua_Init(const std::string& LuaFile)
{
	VX::LuaManager* Manager = new VX::LuaManager(LuaFile);

	return Manager;
}

VX::LuaManager::LuaManager(const std::string& File)
{
	this->LuaState.open_libraries(sol::lib::base, sol::lib::io, sol::lib::math, sol::lib::table, sol::lib::os);
	this->File = File;
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

void VXLua_Destroy(VX::LuaManager*& Manager)
{
	if (Manager)
	{
		delete Manager;
		Manager = nullptr;
	}
}