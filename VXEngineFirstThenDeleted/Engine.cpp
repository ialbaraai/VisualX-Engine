#include "Engine.h"

#include <Windows.h>

VX::Engine* VX_Init(vxs Width, vxs Height, vxm Theme)
{
	ShowWindow(GetConsoleWindow(), SW_HIDE);

	VX::Engine* enginePointer = new VX::Engine();
	enginePointer->choice = '\0';
	enginePointer->input = "";
	enginePointer->width = Width;
	enginePointer->height = Height;
	enginePointer->theme = Theme;

	if (SDL_Init(SDL_INIT_VIDEO))
	{
		enginePointer->is_sdl = true;
	}
	else
	{
		enginePointer->is_sdl = false;
	}

	return enginePointer;
}
VX::Engine* VX_Init()
{
	return VX_Init(1080, 607.5, vxm::Dark);
}
VX::Engine* VX_Init(vxm theme)
{
	return VX_Init(1080, 607.5, theme);
}
void run(VX::Engine* engine)
{
	if (!engine->is_sdl)
	{
		std::cout << "******************************* VX ENGINE *******************************" << '\n';

		do
		{
			do
			{
				std::cout << "What would you like to do ([D]esign or [P]lay or [Q]uit): ";
				std::getline(std::cin >> std::ws, engine->input);
			} while (engine->input.empty());

			if (engine->input.length() == 1)
			{
				engine->choice = toupper(engine->input[0]);

				switch (engine->choice)
				{
				case 'D':
					__VX__Run__Design__(engine);
					break;
				case 'P':
					__VX__Run__Play__();
					break;
				case 'Q':
					break;
				default:
					std::cout << " - Enter a valid input!" << '\n';
					break;
				}
			}
			else
			{
				std::cout << " - Enter only a character!" << '\n';
			}

			if (engine->choice == 'Q')
			{
				do
				{
					std::cout << "Are you sure you want to quit? (Y/N): ";
					std::getline(std::cin >> std::ws, engine->input);
				} while (engine->input.empty());

				engine->choice = toupper(engine->input[0]);

				if (engine->choice == 'Y')
				{
					engine->choice = 'Q';
				}
				else
				{
					engine->choice = '\0';
				}
			}
		} while (engine->choice != 'Q');

		std::cout << "-------------------------------------------------------------------------" << '\n';

		std::cout << "Quitting...\n";
	}
	else
	{
		bool still_running = true;

		VX::Loader* loader = VXLoader_Init(engine->is_sdl);
		bool is_exit = run(loader);
		end(loader);

		if (is_exit)
		{
			return;
		}

		do
		{
			SDL_Window* window = SDL_CreateWindow("VX Engine", engine->width, engine->height, SDL_WINDOW_RESIZABLE);

			if (!window)
			{
				std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << '\n';

				SDL_Quit();
			}
			else
			{
				SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);

				if (!renderer)
				{
					std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << '\n';

					SDL_DestroyWindow(window);
					SDL_Quit();
				}
				else
				{
					SDL_Surface* surface = IMG_Load("VX Engine.jpeg");

					if (surface)
					{
						SDL_SetWindowIcon(window, surface);
						SDL_DestroySurface(surface);
					}

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

					IMGUI_CHECKVERSION();
					ImGuiContext* context = ImGui::CreateContext();

					ImGui::SetCurrentContext(context);

					ImGuiIO& io = ImGui::GetIO();
					ImGuiStyle& style = ImGui::GetStyle();

					io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
					io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

					ImFont* robotoFont = io.Fonts->AddFontFromFileTTF("Roboto.ttf", 16.0f);

					ImVec4 ImGuiCol_ControlButtonColor = { 0.25f, 0.25f, 0.25f, 1.0f };
					ImVec4 ImGuiCol_ControlButtonHoveredColor = { 0.55f, 0.55f, 0.55f, 1.0f };
					ImVec4 ImGuiCol_ControlButtonActiveColor = { 0.4f, 0.4f, 0.4f, 1.0f };

					ImVec4 ImGuiCol_TabColor = { 0.2f, 0.2f, 0.2f, 1.0f };
					ImVec4 ImGuiCol_TabHoveredColor = { 0.4f, 0.4f, 0.4f, 1.0f };
					ImVec4 ImGuiCol_TabActiveColor = { 0.3f, 0.3f, 0.3f, 1.0f };

					bool IsDark = true;

					if (engine->theme == vxm::Dark)
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
					else if (engine->theme == vxm::Light)
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

					ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
					ImGui_ImplSDLRenderer3_Init(renderer);

					bool is_running = true;
					SDL_Event event;

					bool is_failed = false;

					char buff[64] = "";
					char failedBuf[64] = "";

					bool is_create = false;
					bool is_play = false;

					bool is_launch_editor = false;

					while (is_running)
					{
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
								is_running = false;
								still_running = false;
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
						ImGui::Text("VX Engine");
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

						/*ImGui::BulletText("ImGui integration");
						ImGui::BulletText("Lua scripting integration");*/
						ImGui::BulletText("Idk! :)");

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

											if (engine->theme == vxm::Light)
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
												__VX__Run__Play__(game);
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
											
											if (engine->theme == vxm::Light)
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
												strcpy_s(buff, sizeof(buff), game.c_str());
												is_launch_editor = true;
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
							is_running = false;
							still_running = false;
						}
						
						ImGui::PopStyleColor(3);

						ImGui::EndGroup();

						ImGui::End();

						if (is_create)
						{
							ImGui::Begin("Create", &is_create, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoDecoration);

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
								is_launch_editor = true;
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
							ImGui::Begin("Play", &is_play, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoDecoration);

							ImGui::Text("Play");

							ImGui::Text("Filename");
							ImGui::SameLine();
							ImGui::InputText("##PFilename", buff, sizeof(buff));

							static bool is_play_failed = false;

							ImGui::PushStyleColor(ImGuiCol_Button, ImGuiCol_ControlButtonColor);
							ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGuiCol_ControlButtonHoveredColor);
							ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImGuiCol_ControlButtonActiveColor);

							ImVec2 windowSize = ImGui::GetWindowSize();
							ImVec2 buttonSize = {(windowSize.x / 2.0f) - 15.0f, 22.5f};

							if (ImGui::Button("Play", buttonSize))
							{
								if (!__VX__Run__Play__(buff))
								{
									is_play_failed = true;
									strcpy_s(failedBuf, sizeof(buff), buff);
								}
							}
							ImGui::SameLine();
							if (ImGui::Button("Cancel", buttonSize))
							{
								is_play = false;
							}
							ImGui::PopStyleColor(3);

							if (is_play_failed)
							{
								ImGui::TextColored(ImVec4(1.0f, 0.25f, 0.0f, 1.0f), "Failed to load file %s!", failedBuf);
							}

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

					if (is_launch_editor)
					{
						__VX__Run__Design__(engine, std::string(buff));
					}
				}
			}
		} while (still_running);

		SDL_Quit();
	}

	__VX__End__(engine);
}

void __VX__Run__Design__(VX::Engine* Engine, const std::string& filename)
{
	Designer* designer = Designer_Init(filename, Engine->theme);
	Design(designer);
	Designer_Destroy(designer);
}

bool __VX__Run__Play__(const std::string& filename)
{
	RunResult result = RunResult::NextScene;

	Parser* parser = Parser_Init(filename);

	std::string nextFile = "";

	if (Parse(parser))
	{
		do
		{
			if (Parse(parser))
			{
				std::string name = Parser_GetFilePath(parser);
				WindowSize* windowSize = Parser_GetWindowSize(parser);
				Color* windowColor = Parser_GetWindowColor(parser);
				std::vector<Entity*> Entities = Parser_GetEntitiesInfo(parser);
				std::string script = Parser_GetScriptInfo(parser);

				Game* game = Game_Init(name, *windowSize, *windowColor, Entities, script);
				GameLoop(game);
				RunResult result = EndGame(game);

				Parser_Clear(parser);

				if (result == RunResult::NextScene)
				{
					Parser_Pre_Init(parser, nextFile);
				}
			}
		} while (!nextFile.empty());

		return true;
	}
	else
	{
		return false;
	}

	Parser_Destroy(parser);
}

void __VX__End__(VX::Engine* engine)
{
	delete engine;
}