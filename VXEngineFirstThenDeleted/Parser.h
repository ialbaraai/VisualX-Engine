#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include "Entity.h"
#include "EntityComponentSystem.h"
#pragma once

class Parser
{
public:
	bool p_Compilable								  = false;
	std::string p_Filepath							  = "";
	size_t p_MIN_SIZE								  = 1;

	std::map<std::string, int> p_RESERVED_KEYWORDS	  = {};
	std::vector<std::string> p_FILE_CONTENT           = {};

	WindowSize* p_WindowSize						  = new WindowSize(0, 0);
	Color* p_WindowColor                              = new Color(0, 0, 0, 0);

	std::vector<Entity*> p_EntitiesInfo               = {};
	std::string p_ScriptFilepath                      = "";
};

Parser* Parser_Init(const std::string& filename);
void Parser_Pre_Init(Parser* parser, std::string nextFile);

std::vector<std::string> Parser_GetLineInfo(const std::string& line, char splitter);

bool Parse(Parser* parser);

void Parser_SetWindowData(Parser* parser, const std::string& line);
void Parser_SetEntitiesData(Parser* parser, const std::string& line);
void Parser_SetScriptData(Parser* parser, const std::string& line);

std::string Parser_GetFilePath(Parser* parser);
WindowSize* Parser_GetWindowSize(Parser* parser);
Color* Parser_GetWindowColor(Parser* parser);
std::vector<Entity*> Parser_GetEntitiesInfo(Parser* parser);
std::string Parser_GetScriptInfo(Parser* parser);

void Parser_Clear(Parser* parser);

void Parser_Destroy(Parser* parser);