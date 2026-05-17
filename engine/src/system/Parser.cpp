#include "../../include/system/Parser.h"

#include <iostream>
#include <fstream>

std::vector<std::string>& VX::Parser::GetLineInfo(const std::string& line, char splitter)
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

void VX::Parser::SetWindowData(const std::string& line)
{
	try
	{
		std::vector<std::string> lineData = this->GetLineInfo(line, ',');

		this->p_WindowSize.X = std::stoi(lineData[0]);
		this->p_WindowSize.Y = std::stoi(lineData[1]);

		this->p_WindowColor.Red = std::stoi(lineData[2]);
		this->p_WindowColor.Green = std::stoi(lineData[3]);
		this->p_WindowColor.Blue = std::stoi(lineData[4]);
	}
	catch (const std::exception& e)
	{
		// throw std::exception("Invalid window data!\n");
	}
}
void VX::Parser::SetEntitiesData(const std::string& line)
{
	try
	{
		std::vector<std::string> lineData = this->GetLineInfo(line, ',');

		std::string EntityName = lineData[0];
		int EntityId = std::stoi(lineData[1]);

		for (const Entity& entity : this->p_EntitiesInfo)
		{
			if (entity.Get_Id() == EntityId)
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

		Entity entity(EntityId, EntityName, Position(PosX, PosY), Size(SizeX, SizeY), Color(ColorR, ColorG, ColorB, 255), SpriteComponent(isImage, Image));

		this->p_EntitiesInfo.push_back(entity);
	}
	catch (const std::exception& e)
	{
		// throw std::exception("Invalid entity data!\n");
	}
}
void VX::Parser::SetScriptData(const std::string& line)
{
	this->p_ScriptFilepath = line;
}

VX::Parser::Parser(const std::string& filename)
{
	this->p_RESERVED_KEYWORDS["$/EOWDTED/$"] = 1;
	this->p_RESERVED_KEYWORDS["$/EOEDTSFP/$"] = 2;
	this->p_RESERVED_KEYWORDS["$/EOGD/$"] = 3;

	if (filename.empty())
	{
		do
		{
			std::cout << "Enter file name (file.vx, e.g. \"game.vx\"): ";
			std::getline(std::cin >> std::ws, this->p_Filepath);
		} while (this->p_Filepath.empty());
	}
	else
	{
		this->p_Filepath = filename;
	}
}

bool VX::Parser::parse()
{
	std::ifstream output;
	output.open(this->p_Filepath);

	if (output.is_open())
	{
		std::string curLine = "";

		while (std::getline(output, curLine))
		{
			this->p_FILE_CONTENT.push_back(curLine);
		}
		
		output.close();

		try
		{
			if (this->p_FILE_CONTENT.size() > this->p_MIN_SIZE)
			{
				int mode = 0;

				for (size_t i = 0; i < this->p_FILE_CONTENT.size(); i++)
				{
					bool is_reserved = false;

					for (auto& reserved : this->p_RESERVED_KEYWORDS)
					{
						if (reserved.first == this->p_FILE_CONTENT[i])
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
								this->SetWindowData(this->p_FILE_CONTENT[i]);
								break;
							case 1:
								this->SetEntitiesData(this->p_FILE_CONTENT[i]);
								break;
							case 2:
								this->SetScriptData(this->p_FILE_CONTENT[i]);
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
		this->p_Compilable = false;
		std::cout << "Couldn't open file path to parse..." << std::endl;
		return false;
	}
}

std::string& VX::Parser::Get_FilePath()
{
	return this->p_Filepath;
}
WindowSize& VX::Parser::Get_WindowSize()
{
	return this->p_WindowSize;
}
Color& VX::Parser::Get_WindowColor()
{
	return this->p_WindowColor;
}
std::vector<Entity>& VX::Parser::Get_Entities()
{
	return this->p_EntitiesInfo;
}
std::string& VX::Parser::Get_Script()
{
	return this->p_ScriptFilepath;
}

void VX::Parser::clear()
{
	this->p_EntitiesInfo.clear();
}