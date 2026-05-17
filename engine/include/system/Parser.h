#pragma once

#include <vector>
#include <string>
#include <map>

#include "Entity.h"
#include "EntityComponentSystem.h"

namespace VX
{
	class Parser
	{
	private:
		bool p_Compilable = false;
		std::string p_Filepath = "";
		size_t p_MIN_SIZE = 1;

		std::map<std::string, int> p_RESERVED_KEYWORDS = {};
		std::vector<std::string> p_FILE_CONTENT = {};

		WindowSize p_WindowSize;
		Color p_WindowColor;

		std::vector<Entity> p_EntitiesInfo = {};
		std::string p_ScriptFilepath = "";

		std::vector<std::string>& GetLineInfo(const std::string& line, char splitter);

		void SetWindowData(const std::string& line);
		void SetEntitiesData(const std::string& line);
		void SetScriptData(const std::string& line);

	public:
		Parser(const std::string& filename);
		bool parse();
		
		std::string& Get_FilePath();
		WindowSize& Get_WindowSize();
		Color& Get_WindowColor();
		std::vector<Entity>& Get_Entities();
		std::string& Get_Script();

		void clear();
	};
}