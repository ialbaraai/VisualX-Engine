#pragma once

#include <iostream>
#include <type_traits>
#include <fstream>
#include <filesystem>
#include <string>
#include <cstring>
#include <vector>
#include <map>

#include "../system/Backend.h"
#include "../system/Entity.h"
#include "../system/EntityComponentSystem.h"
#include "../system/Parser.h"
#include "../utilities/VXLua.h"

class Designer
{
public:
	SDL_Window* p_Window			= nullptr;
	SDL_Renderer* p_Renderer		= nullptr;
	vxm p_Theme;

	std::string p_Filepath			= "";

	std::vector<std::string> p_Data = {};
};

Designer Designer_Init(const std::string& Filepath, vxm Theme);

void Design(Designer* designer);
void Apply(Designer* designer, WindowSize* windowSize, Color* windowColor, const std::string& scriptFilepath, const std::vector<Entity*>& entities);
void Designer_Destroy(Designer* designer);