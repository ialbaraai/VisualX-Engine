#include "../../include/system/Parser.h"

Parser* Parser_Init(const std::string& filename)
{
	Parser* parser = new Parser();
	parser->p_RESERVED_KEYWORDS["$/EOWDTED/$"] = 1;
	parser->p_RESERVED_KEYWORDS["$/EOEDTSFP/$"] = 2;
	parser->p_RESERVED_KEYWORDS["$/EOGD/$"] = 3;
	

	if (filename.empty())
	{
		do
		{
			std::cout << "Enter file name (file.vx, e.g. \"game.vx\"): ";
			std::getline(std::cin >> std::ws, parser->p_Filepath);
		} while (parser->p_Filepath.empty());
	}
	else
	{
		parser->p_Filepath = filename;
	}

	return parser;
}
void Parser_Pre_Init(Parser* parser, std::string nextFile)
{
	parser->p_Filepath = nextFile;
}

std::vector<std::string> Parser_GetLineInfo(const std::string& line, char splitter)
{
	std::vector<std::string> output = {};

	std::string string;

	for (int i = 0; i < line.length(); i++)
	{
		if (line[i] != splitter)
		{
			string += line[i];
		}
		else
		{
			output.push_back(string);
			string.clear();
		}
	}

	output.push_back(string);

	return output;
}

bool Parse(Parser* parser)
{
	std::ifstream output;
	output.open(parser->p_Filepath);

	if (output.is_open())
	{
		std::string curLine = "";

		while (std::getline(output, curLine))
		{
			parser->p_FILE_CONTENT.push_back(curLine);
		}
		
		output.close();

		try
		{
			if (parser->p_FILE_CONTENT.size() > parser->p_MIN_SIZE)
			{
				int mode = 0;

				for (size_t i = 0; i < parser->p_FILE_CONTENT.size(); i++)
				{
					bool is_reserved = false;

					for (auto& reserved : parser->p_RESERVED_KEYWORDS)
					{
						if (reserved.first == parser->p_FILE_CONTENT[i])
						{
							mode = reserved.second;
							is_reserved = true;
							break;
						}
					}

					if (is_reserved == false)
					{
						switch (mode)
						{
							case 0:
								Parser_SetWindowData(parser, parser->p_FILE_CONTENT[i]);
								break;
							case 1:
								Parser_SetEntitiesData(parser, parser->p_FILE_CONTENT[i]);
								break;
							case 2:
								Parser_SetScriptData(parser, parser->p_FILE_CONTENT[i]);
								break;
							default:
								break;
						}
					}
					else
					{
						if (mode == 3)
						{
							break;
						}
					}
				}
			}
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
			return false;
		}

		return true;
	}
	else
	{
		parser->p_Compilable = false;
		std::cout << "Couldn't open file path to parse..." << std::endl;
		return false;
	}
}


void Parser_SetWindowData(Parser* parser, const std::string& line)
{
	try
	{
		std::vector<std::string> lineData = Parser_GetLineInfo(line, ',');

		parser->p_WindowSize->X = std::stoi(lineData[0]);
		parser->p_WindowSize->Y = std::stoi(lineData[1]);

		parser->p_WindowColor->Red = std::stoi(lineData[2]);
		parser->p_WindowColor->Green = std::stoi(lineData[3]);
		parser->p_WindowColor->Blue = std::stoi(lineData[4]);
	}
	catch (const std::exception& e)
	{
		// throw std::exception("Invalid window data!\n");
	}
}
void Parser_SetEntitiesData(Parser* parser, const std::string& line)
{
	try
	{
		std::vector<std::string> lineData = Parser_GetLineInfo(line, ',');

		std::string EntityName = lineData[0];
		int EntityId = std::stoi(lineData[1]);

		for (Entity* entity : parser->p_EntitiesInfo)
		{
			if (entity->Get_Id() == EntityId)
			{
				return;
			}
		}

		int SizeX = std::stoi(lineData[2]);
		int SizeY = std::stoi(lineData[3]);

		int PosX = std::stoi(lineData[4]);
		int PosY = std::stoi(lineData[5]);

		int ColorR = std::stoi(lineData[6]);
		int ColorG = std::stoi(lineData[7]);
		int ColorB = std::stoi(lineData[8]);

		int IsImage = std::stoi(lineData[9]);
		std::string Image = lineData[10];

		bool isImage = IsImage == 1 ? true : false;

		Entity* entity = new Entity(EntityId, EntityName, Position(PosX, PosY), Size(SizeX, SizeY), Color(ColorR, ColorG, ColorB, 255), SpriteComponent(isImage, Image));

		parser->p_EntitiesInfo.push_back(entity);
	}
	catch (const std::exception& e)
	{
		// throw std::exception("Invalid entity data!\n");
	}
}
void Parser_SetScriptData(Parser* parser, const std::string& line)
{
	parser->p_ScriptFilepath = line;
}

std::string Parser_GetFilePath(Parser* parser)
{
	return parser->p_Filepath;
}
WindowSize* Parser_GetWindowSize(Parser* parser)
{
	return parser->p_WindowSize;
}
Color* Parser_GetWindowColor(Parser* parser)
{
	return parser->p_WindowColor;
}
std::vector<Entity*> Parser_GetEntitiesInfo(Parser* parser)
{
	return parser->p_EntitiesInfo;
}
std::string Parser_GetScriptInfo(Parser* parser)
{
	return parser->p_ScriptFilepath;
}

void Parser_Clear(Parser* parser)
{
	parser->p_EntitiesInfo.clear();
}

void Parser_Destroy(Parser* parser)
{
	delete parser;
}