#include "../../include/application/Engine.h"

#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include <cstring>

#include <ctype.h>

#include "../../include/system/Backend.h"
#include "../../include/application/Game.h"
#include "../../include/application/Designer.h"
#include "../../include/application/Loader.h"
#include "../../include/system/Parser.h"

#define QUIT_SELECTION 0
#define EDITOR_SELECTION 1
#define GAME_SELECTION 2

VX::Engine::Engine()
{
	this->choice = '\0';
	this->input = "";
	this->width = 1000;
	this->height = 600;
	this->theme = vxm::Dark;
	this->selection = 0;
}

int VX::Engine::run()
{
	do
	{
		this->engine_running = true;

		if (!SDL_Init(SDL_INIT_VIDEO))
		{
			std::cerr << "Error while initializing SDL: " << SDL_GetError() << '\n';

			return 1;
		}

		SDL_Window* window = SDL_CreateWindow("VX Engine", this->width, this->height, SDL_WINDOW_RESIZABLE);

		if (!window)
		{
			std::cerr << "Error while creating window: " << SDL_GetError() << '\n';

			SDL_Quit();

			return 1;
		}

		SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);

		if (!renderer)
		{
			std::cout << "Error while creating rendererer: " << SDL_GetError() << '\n';

			SDL_DestroyWindow(window);
			SDL_Quit();
			
			return 1;
		}

		SDL_Surface* surface = IMG_Load("assets/VX Engine.jpeg");

		if (surface)
		{
			SDL_SetWindowIcon(window, surface);
			SDL_DestroySurface(surface);
		}

		IMGUI_CHECKVERSION();
		ImGuiContext* context = ImGui::CreateContext();
		ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
		ImGui_ImplSDLRenderer3_Init(renderer);

		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();

		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

		ImVec4 ImGuiCol_ControlButtonColor = { 0.25f, 0.25f, 0.25f, 1.0f };
		ImVec4 ImGuiCol_ControlButtonHoveredColor = { 0.55f, 0.55f, 0.55f, 1.0f };
		ImVec4 ImGuiCol_ControlButtonActiveColor = { 0.4f, 0.4f, 0.4f, 1.0f };

		ImVec4 ImGuiCol_TabColor = { 0.2f, 0.2f, 0.2f, 1.0f };
		ImVec4 ImGuiCol_TabHoveredColor = { 0.4f, 0.4f, 0.4f, 1.0f };
		ImVec4 ImGuiCol_TabActiveColor = { 0.3f, 0.3f, 0.3f, 1.0f };

		bool IsDark = true;

		if (this->theme == vxm::Dark)
		{
			ImGui::StyleColorsDark();

			ImGuiCol_ControlButtonColor = DARK_THEME_ImGuiCol_ControlButtonColor;
			ImGuiCol_ControlButtonHoveredColor = DARK_THEME_ImGuiCol_ControlButtonHoveredColor;
			ImGuiCol_ControlButtonActiveColor = DARK_THEME_ImGuiCol_ControlButtonActiveColor;

			ImGuiCol_TabColor = { 0.2f, 0.2f, 0.2f, 1.0f };
			ImGuiCol_TabHoveredColor = { 0.4f, 0.4f, 0.4f, 1.0f };
			ImGuiCol_TabActiveColor = { 0.3f, 0.3f, 0.3f, 1.0f };

			style.Colors[ImGuiCol_WindowBg] = DARK_THEME_Background_Color;

			IsDark = true;
		}
		else if (this->theme == vxm::Light)
		{
			ImGui::StyleColorsLight();

			ImGuiCol_ControlButtonColor = LIGHT_THEME_ImGuiCol_ControlButtonColor;
			ImGuiCol_ControlButtonHoveredColor = LIGHT_THEME_ImGuiCol_ControlButtonHoveredColor;
			ImGuiCol_ControlButtonActiveColor = LIGHT_THEME_ImGuiCol_ControlButtonActiveColor;

			ImGuiCol_TabColor = { 0.90f, 0.90f, 0.90f, 1.0f };
			ImGuiCol_TabHoveredColor = { 0.75f, 0.75f, 0.75f, 1.0f };
			ImGuiCol_TabActiveColor = { 0.80f, 0.80f, 0.80f, 1.0f };

			IsDark = false;
		}
		else
		{
			ImGui::StyleColorsClassic();

			IsDark = true;
		}

		ImFont* robotoFont = io.Fonts->AddFontFromFileTTF("assets/Roboto.ttf", 16.0f);

		bool GetGamesFailure = false;
		bool NoGames = false;

		std::vector<std::string> games = {};

		std::ifstream inputGames;
		inputGames.open("games.txt");

		if (inputGames.is_open())
		{
			std::string curGame;

			while (std::getline(inputGames, curGame))
			{
				if (curGame.empty()) continue;

				bool is_in = false;

				for (const std::string& cur : games)
				{
					if (curGame == cur)
					{
						is_in = true;
					}
				}

				if (!is_in)
				{
					games.push_back(curGame);
				}
			}

			inputGames.close();
		}
		else
		{
			GetGamesFailure = true;
		}

		if (games.size() < 1)
		{
			NoGames = true;
		}

		bool is_running = true;
		SDL_Event event;
		char buff[64] = "";

		bool is_create = false; // for create menu (NOT EDITOR LAUNCH)
		bool is_play = false; // for play menu (NOT GAME LAUNCH)

		while (is_running)
		{
			// IN CASE OF EDITOR OR GAME WE MAKE `is_running = false`

			int WindowWidth, WindowHeight;
			SDL_GetWindowSize(window, &WindowWidth, &WindowHeight);

			if (WindowWidth <= 0 || WindowHeight <= 0)
			{
				continue;
			}

			while (SDL_PollEvent(&event))
			{
				ImGui_ImplSDL3_ProcessEvent(&event);

				if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
				{
					this->selection = QUIT_SELECTION;
					is_running = false;
					this->engine_running = false;
				}
			}

			ImGui_ImplSDL3_NewFrame();
			ImGui_ImplSDLRenderer3_NewFrame();
			ImGui::NewFrame();

			ImGui::PushFont(robotoFont);

			ImGui::SetNextWindowSize(ImVec2(ImGui::GetMainViewport()->Size.x, 50.0f));
			ImGui::SetNextWindowPos(ImVec2(0, 0));
			ImGui::Begin("Control Panel", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration);

			ImGui::Dummy(ImVec2(0, 5.0f));

			ImGui::BeginGroup();

			ImGui::PushItemWidth(50.0f);
			ImGui::Text("VX this");
			ImGui::PopItemWidth();

			ImGui::EndGroup();

			ImGui::SameLine();
			ImGui::Dummy(ImVec2(ImGui::GetMainViewport()->Size.x - (75.0f * 3 + 10.0f * 2 + 50.0f), 0));
			ImGui::SameLine();

			ImGui::BeginGroup();

			ImGui::PushStyleColor(ImGuiCol_Button, ImGuiCol_ControlButtonColor);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGuiCol_ControlButtonHoveredColor);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImGuiCol_ControlButtonActiveColor);

			if (ImGui::Button("Create", ImVec2(75.0f, 0.0f)))
			{
				is_create = !is_create;
				is_play = false;
			}

			ImGui::SameLine();
			ImGui::Dummy(ImVec2(10.0f, 0.0f));
			ImGui::SameLine();

			if (ImGui::Button("Play", ImVec2(75.0f, 0.0f)))
			{
				is_play = !is_play;
				is_create = false;
			}

			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::Dummy(ImVec2(10.0f, 0.0f));
			ImGui::EndGroup();

			ImGui::Dummy(ImVec2(0, 5.0f));

			ImGui::End();

			ImGui::SetNextWindowSize(ImVec2(ImGui::GetMainViewport()->Size.x / 5, ImGui::GetMainViewport()->Size.y - (100.0f)));
			ImGui::SetNextWindowPos(ImVec2(0, 50.0f));
			ImGui::Begin("Side", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);

			ImGui::Text("Releases");

			ImGui::BulletText("Lua scripting integration");

			ImGui::End();

			ImGui::SetNextWindowSize(ImVec2((ImGui::GetMainViewport()->Size.x / 5) * 4, ImGui::GetMainViewport()->Size.y - (100.0f)));
			ImGui::SetNextWindowPos(ImVec2(ImGui::GetMainViewport()->Size.x / 5, 50.0f));
			ImGui::Begin("Projects", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);

			ImGui::Text("Projects");

			ImGui::BeginGroup();

			ImGui::PushStyleColor(ImGuiCol_Tab, ImGuiCol_TabColor);
			ImGui::PushStyleColor(ImGuiCol_TabHovered, ImGuiCol_TabHoveredColor);
			ImGui::PushStyleColor(ImGuiCol_TabActive, ImGuiCol_TabActiveColor);
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(30, 3));
			if (ImGui::BeginTabBar("ProjectsBar", ImGuiTabBarFlags_Reorderable))
			{
				if (ImGui::BeginTabItem("Play", nullptr, ImGuiTabItemFlags_NoCloseWithMiddleMouseButton))
				{
					ImGui::Dummy(ImVec2(0.0f, 10.0f));

					if (!GetGamesFailure)
					{
						if (!NoGames)
						{
							for (const std::string& game : games)
							{
								ImGui::Dummy(ImVec2(5.0f, 0.0f));
								ImGui::SameLine();

								if (this->theme == vxm::Light)
								{
									ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.75f, 0.75f, 0.75f, 1.0f));        // normal gray
									ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.60f, 0.60f, 0.60f, 1.0f));  // slightly darker
									ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.85f, 0.85f, 0.85f, 1.0f));
								}
								else
								{
									ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.20f, 0.22f, 0.26f, 1.0f));        // normal gray
									ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.30f, 0.33f, 0.38f, 1.0f));  // slightly lighter
									ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.25f, 0.28f, 0.32f, 1.0f));
								}

								if (ImGui::Button(game.c_str(), ImVec2((ImGui::GetMainViewport()->Size.x / 5) * 3.75, 50.0f)))
								{
									strncpy(buff, game.c_str(), sizeof(buff));
									this->selection = GAME_SELECTION;
									is_running = false;
								}
								ImGui::PopStyleColor(3);

								ImGui::Dummy(ImVec2(0.0f, 10.0f));
							}
						}
						else
						{
							ImGui::Dummy(ImVec2(5.0f, 0.0f));
							ImGui::SameLine();
							ImGui::TextColored(ImVec4(1.0f, 0.35f, 0.0f, 1.0f), "No saved projects");
						}
					}
					else
					{
						ImGui::Dummy(ImVec2(5.0f, 0.0f));
						ImGui::SameLine();
						ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Failed to load projects");
					}

					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Edit", nullptr, ImGuiTabItemFlags_NoCloseWithMiddleMouseButton))
				{
					ImGui::Dummy(ImVec2(0.0f, 10.0f));

					if (!GetGamesFailure)
					{
						if (!NoGames)
						{
							for (const std::string& game : games)
							{
								ImGui::Dummy(ImVec2(5.0f, 0.0f));
								ImGui::SameLine();
								
								if (this->theme == vxm::Light)
								{
									ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.70f, 0.70f, 0.70f, 1.0f));        // normal gray
									ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.55f, 0.55f, 0.55f, 1.0f));  // hover
									ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.80f, 0.80f, 0.80f, 1.0f));
								}
								else
								{
									ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.25f, 0.27f, 0.31f, 1.0f));        // normal gray
									ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.35f, 0.38f, 0.43f, 1.0f));  // slightly lighter
									ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.30f, 0.33f, 0.37f, 1.0f));
								}

								if (ImGui::Button(game.c_str(), ImVec2((ImGui::GetMainViewport()->Size.x / 5) * 3.75, 50.0f)))
								{
									strncpy(buff, game.c_str(), sizeof(buff));
									this->selection = EDITOR_SELECTION;
									is_running = false;
								}
								ImGui::PopStyleColor(3);

								ImGui::Dummy(ImVec2(0.0f, 10.0f));
							}
						}
						else
						{
							ImGui::Dummy(ImVec2(5.0f, 0.0f));
							ImGui::SameLine();
							ImGui::TextColored(ImVec4(1.0f, 0.35f, 0.0f, 1.0f), "No saved projects");
						}
					}
					else
					{
						ImGui::Dummy(ImVec2(5.0f, 0.0f));
						ImGui::SameLine();
						ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Failed to load projects");
					}

					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
			}
			ImGui::PopStyleColor(3);
			ImGui::PopStyleVar();

			ImGui::EndGroup();

			ImGui::End();

			ImGui::SetNextWindowSize(ImVec2(ImGui::GetMainViewport()->Size.x, 50.0f));
			ImGui::SetNextWindowPos(ImVec2(0.0f, ImGui::GetMainViewport()->Size.y - 50.0f));
			ImGui::Begin("Footer", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration);

			ImGui::Dummy(ImVec2(0.0f, 5.0f));

			ImGui::BeginGroup();

			ImGui::PushItemWidth(150.0f);
			io.FontGlobalScale = 0.5f;
			ImGui::Text("Credits: Me. coffee. milk.");
			io.FontGlobalScale = 1.0f;
			ImGui::PopItemWidth();

			ImGui::EndGroup();

			ImGui::SameLine();
			ImGui::Dummy(ImVec2(ImGui::GetMainViewport()->Size.x - (200.0f + 75.0f + 5.0f), 0));
			ImGui::SameLine();

			ImGui::BeginGroup();

			ImGui::PushStyleColor(ImGuiCol_Button, ImGuiCol_ControlButtonColor);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGuiCol_ControlButtonHoveredColor);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImGuiCol_ControlButtonActiveColor);

			if (ImGui::Button("Quit", ImVec2(75.0f, 0.0f)))
			{
				this->selection = QUIT_SELECTION;
				is_running = false;
				this->engine_running = false;
			}
			
			ImGui::PopStyleColor(3);

			ImGui::EndGroup();

			ImGui::End();

			if (is_create)
			{
				ImGui::Begin("Create", &is_create, ImGuiWindowFlags_NoDecoration);

				ImGui::Text("Create");

				ImGui::Text("Filename");
				ImGui::SameLine();
				ImGui::InputText("##CFilename", buff, sizeof(buff));

				ImGui::PushStyleColor(ImGuiCol_Button, ImGuiCol_ControlButtonColor);
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGuiCol_ControlButtonHoveredColor);
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImGuiCol_ControlButtonActiveColor);

				ImVec2 windowSize = ImGui::GetWindowSize();
				ImVec2 buttonSize = { (windowSize.x / 2.0f) - 15.0f, 22.5f };

				if (ImGui::Button("Create", buttonSize))
				{
					is_running = false;
					this->selection = EDITOR_SELECTION;
				}
				ImGui::SameLine();
				if (ImGui::Button("Cancel", buttonSize))
				{
					is_create = false;
				}
				ImGui::PopStyleColor(3);

				ImGui::End();
			}

			if (is_play)
			{
				ImGui::Begin("Play", &is_play, ImGuiWindowFlags_NoDecoration);

				ImGui::Text("Play");

				ImGui::Text("Filename");
				ImGui::SameLine();
				ImGui::InputText("##PFilename", buff, sizeof(buff));

				ImGui::PushStyleColor(ImGuiCol_Button, ImGuiCol_ControlButtonColor);
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGuiCol_ControlButtonHoveredColor);
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImGuiCol_ControlButtonActiveColor);

				ImVec2 windowSize = ImGui::GetWindowSize();
				ImVec2 buttonSize = {(windowSize.x / 2.0f) - 15.0f, 22.5f};

				if (ImGui::Button("Play", buttonSize))
				{
					is_running = false;
					this->selection = GAME_SELECTION;
				}
				ImGui::SameLine();
				if (ImGui::Button("Cancel", buttonSize))
				{
					is_play = false;
				}
				ImGui::PopStyleColor(3);

				ImGui::End();
			}

			ImGui::PopFont();

			ImGui::Render();

			SDL_SetRenderDrawColor(renderer, 100, 100, 125, 155);
			SDL_RenderClear(renderer);

			if (ImGui::GetDrawData())
			{
				ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
			}

			SDL_RenderPresent(renderer);

		}

		ImGui_ImplSDLRenderer3_Shutdown();
		ImGui_ImplSDL3_Shutdown();
		ImGui::DestroyContext();

		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();

		if (this->selection == EDITOR_SELECTION)
		{
			VX::Designer designer(std::string(buff), this->theme);
			designer.run();
		}
		if (this->selection == GAME_SELECTION)
		{
			std::cout << "game selection mode!\n";
			VX::Parser parser{std::string(buff)};

			std::cout << "made parser!\n";
			if (parser.parse())
			{
				std::cout << "read file: " << parser.Get_FilePath() << "!\n";
				std::string name = parser.Get_FilePath();
				WindowSize windowSize = parser.Get_WindowSize();
				Color windowColor = parser.Get_WindowColor();
				std::vector<Entity> Entities = parser.Get_Entities();
				std::string script = parser.Get_Script();
				std::cout << "retrieved data!\n";

				VX::Game game;
				std::cout << "made game!\n";
				game.loop(name, windowSize, windowColor, Entities, script);
				std::cout << "ran game!\n";

				parser.clear();
			}
		}
	} while (this->engine_running);
}