#pragma once

#define SOL_ALL_SAFETIES_ON 1

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include <cstring>

#include <ctype.h>
#include <stdbool.h>

#include "Backend.h"
#include "Game.h"
#include "Designer.h"
#include "Loader.h"
#include "Parser.h"

namespace VX
{
	class Engine
	{
	public:
		std::string input = "";
		char choice;
		bool is_sdl;
		vxs width;
		vxs height;
		vxm theme;
	};
};

VX::Engine* VX_Init(vxs, vxs, vxm);
VX::Engine* VX_Init();
VX::Engine* VX_Init(vxm);
void run(VX::Engine*);

void __VX__Run__Design__(VX::Engine*, const std::string& = "");
bool __VX__Run__Play__(const std::string& = "");

void __VX__End__(VX::Engine*);