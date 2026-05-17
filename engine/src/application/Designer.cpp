#include "../../include/application/Designer.h"

#include <iostream>
#include <type_traits>
#include <fstream>
#include <filesystem>
#include <string>
#include <cstring>

#include "../../include/system/Parser.h"

VX::Designer::Designer(const std::string& Filepath, vxm Theme)
{
	this->p_Theme = Theme;

	if (Filepath.empty())
	{
		std::string filepath;

		do
		{
			std::cout << "Enter filename: ";
			std::getline(std::cin >> std::ws, filepath);
		} while (filepath.empty());

		this->p_Filepath = filepath;
	}
	else
	{
		this->p_Filepath = Filepath;
	}
}

void VX::Designer::run()
{
	std::string filepath = this->p_Filepath;
	std::string title = "VX Editor - " + this->p_Filepath;
	
	this->p_Window = SDL_CreateWindow(title.c_str(), 1440, 847, SDL_WINDOW_RESIZABLE);

	if (this->p_Window)
	{
		SDL_Surface* surface = IMG_Load("assets/VX Engine.jpeg");

		if (surface)
		{
			SDL_SetWindowIcon(this->p_Window, surface);
			SDL_DestroySurface(surface);
		}

		this->p_Renderer = SDL_CreateRenderer(this->p_Window, nullptr);

		if (this->p_Renderer)
		{
			IMGUI_CHECKVERSION();
			ImGuiContext* context = ImGui::CreateContext();
			ImGui::SetCurrentContext(context);

			ImGuiIO& io = ImGui::GetIO();
			ImGuiStyle& style = ImGui::GetStyle();

			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

			ImVec4 ImGuiCol_ControlButtonColor = { 0.25f, 0.25f, 0.25f, 1.0f };
			ImVec4 ImGuiCol_ControlButtonHoveredColor = { 0.55f, 0.55f, 0.55f, 1.0f };
			ImVec4 ImGuiCol_ControlButtonActiveColor = { 0.4f, 0.4f, 0.4f, 1.0f };

			bool IsDark = true;

			if (this->p_Theme == vxm::Dark)
			{
				ImGui::StyleColorsDark();

				ImGuiCol_ControlButtonColor = DARK_THEME_ImGuiCol_ControlButtonColor;
				ImGuiCol_ControlButtonHoveredColor = DARK_THEME_ImGuiCol_ControlButtonHoveredColor;
				ImGuiCol_ControlButtonActiveColor = DARK_THEME_ImGuiCol_ControlButtonActiveColor;

				style.Colors[ImGuiCol_WindowBg] = DARK_THEME_Background_Color;

				IsDark = true;
			}
			else if (this->p_Theme == vxm::Light)
			{
				ImGui::StyleColorsLight();

				ImGuiCol_ControlButtonColor = LIGHT_THEME_ImGuiCol_ControlButtonColor;
				ImGuiCol_ControlButtonHoveredColor = LIGHT_THEME_ImGuiCol_ControlButtonHoveredColor;
				ImGuiCol_ControlButtonActiveColor = LIGHT_THEME_ImGuiCol_ControlButtonActiveColor;

				IsDark = false;
			}
			else
			{
				ImGui::StyleColorsClassic();

				IsDark = true;
			}

			ImFont* robotoFont = io.Fonts->AddFontFromFileTTF("assets/Roboto.ttf", 16.0f);

			ImGui_ImplSDL3_InitForSDLRenderer(this->p_Window, this->p_Renderer);
			ImGui_ImplSDLRenderer3_Init(this->p_Renderer);

			bool is_running = true;
			SDL_Event event;

			Parser parser(filepath);

			SDL_Texture* GameScene = SDL_CreateTexture(this->p_Renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, 800, 600);
			ImVec2 sceneOffset = { 0, 0 };

			Entity* Target = nullptr;
			size_t TargetIndex = -1;

			Color SceneBackgroundColors(0, 0, 0, 255);

			std::vector<Entity> entities = {};
			std::string IntegratedScriptFilepath = "";

			if (parser.parse())
			{
				WindowSize& gameSceneSize = parser.Get_WindowSize();
				Color& ParsedSceneBackgroundColors = parser.Get_WindowColor();
				entities = parser.Get_Entities();
				IntegratedScriptFilepath = parser.Get_Script();

				SceneBackgroundColors.Red = ParsedSceneBackgroundColors.Red;
				SceneBackgroundColors.Green = ParsedSceneBackgroundColors.Green;
				SceneBackgroundColors.Blue = ParsedSceneBackgroundColors.Blue;
				SceneBackgroundColors.Alpha = ParsedSceneBackgroundColors.Alpha;

				int SceneWidth = gameSceneSize.X < 500 ? 500 : gameSceneSize.X;
				int SceneHeight = gameSceneSize.Y < 500 ? 500 : gameSceneSize.Y;

				for (Entity& entity : entities)
				{
					SpriteComponent::Apply(&entity.Get_Sprite(), this->p_Renderer);
				}

				SDL_DestroyTexture(GameScene);
				GameScene = SDL_CreateTexture(this->p_Renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, SceneWidth, SceneHeight);
			}

			SDL_SetTextureBlendMode(GameScene, SDL_BLENDMODE_BLEND);

			bool is_log_loop = false;

			while (is_running)
			{
				int WindowWidth, WindowHeight;
				SDL_GetWindowSize(this->p_Window, &WindowWidth, &WindowHeight);

				float SceneWidth, SceneHeight;
				SDL_GetTextureSize(GameScene, &SceneWidth, &SceneHeight);

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
					}
					if (event.type == SDL_EVENT_KEY_UP)
					{
						switch (event.key.key)
						{
						case SDLK_DELETE:
							if (Target)
							{
								for (size_t i = 0; i < entities.size(); i++)
								{
									if (&entities[i] == Target)
									{
										entities.erase(entities.begin() + i);
										Target = nullptr;
										TargetIndex = -1;
									}
								}
							}
						}
					}
				}

				ImGui::PushFont(robotoFont);

				static bool IsCreateWindow = false;
				static bool IsAddScriptWindow = false;

				ImTextureID SceneId = (ImTextureID)GameScene;

				int Red = 0, Green = 0, Blue = 0, Alpha = 0;

				if (Target)
				{
					Red = Target->Get_Color().Red;
					Green = Target->Get_Color().Green;
					Blue = Target->Get_Color().Blue;
					Alpha = Target->Get_Color().Alpha;
				}

				ImGui_ImplSDL3_NewFrame();
				ImGui_ImplSDLRenderer3_NewFrame();
				ImGui::NewFrame();

				ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
				ImGui::SetNextWindowSize(ImVec2(250.0f, ImGui::GetMainViewport()->Size.y - 400.0f), ImGuiCond_Always);
				ImGui::Begin("Objects Window", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration);

				ImGui::Text("Objects");

				ImGui::Dummy(ImVec2(0.0f, 10.0f));
				ImGui::Separator();
				ImGui::Dummy(ImVec2(0.0f, 10.0f));

				ImGui::BeginGroup();

				for (size_t index = 0; index < entities.size(); index++)
				{
					Entity& entity = entities.at(index);

					if (index >= 1)
					{
						ImGui::Dummy(ImVec2(0.0f, 2.5f));
					}

					if (ImGui::Selectable(entity.Get_Name().c_str(), Target == &entity))
					{
						Target = &entity;
						TargetIndex = index;

						Red = Target->Get_Color().Red;
						Green = Target->Get_Color().Green;
						Blue = Target->Get_Color().Blue;
						Alpha = Target->Get_Color().Alpha;
					}
				}

				ImGui::EndGroup();

				ImGui::End();

				ImGui::SetNextWindowPos(ImVec2(0.0f, ImGui::GetMainViewport()->Size.y - 400.0f), ImGuiCond_Always);
				ImGui::SetNextWindowSize(ImVec2(250.0f, 60.0f), ImGuiCond_Always);
				ImGui::Begin("Script Window", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration);

				ImGui::Dummy(ImVec2(0.0f, 10.0f));

				if (!IntegratedScriptFilepath.empty())
				{
					ImGui::Text(IntegratedScriptFilepath.c_str());
				}
				else
				{
					ImGui::TextColored(ImVec4(0.7f, 0.1f, 0.1f, 1.0f), "No script integrated");
				}

				ImGui::Dummy(ImVec2(0.0f, 10.0f));

				ImGui::End();

				ImGui::SetNextWindowPos(ImVec2(0.0f, ImGui::GetMainViewport()->Size.y - 340.0f), ImGuiCond_Always);
				ImGui::SetNextWindowSize(ImVec2(250.0f, 90.0f), ImGuiCond_Always);
				ImGui::Begin("Create Window", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration);

				ImGui::Dummy(ImVec2(0.0f, 10.0f));

				float CreateToggleButtonSize = ImGui::GetWindowWidth() - 20.0f;
				ImGui::PushStyleColor(ImGuiCol_Button, ImGuiCol_ControlButtonColor);
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGuiCol_ControlButtonHoveredColor);
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImGuiCol_ControlButtonActiveColor);
				if (ImGui::Button("Create", ImVec2(CreateToggleButtonSize, 20.0f)))
				{
					IsCreateWindow = !IsCreateWindow;
					IsAddScriptWindow = false;
				}
				if (ImGui::Button("Add Script", ImVec2(CreateToggleButtonSize, 20.0f)))
				{
					IsAddScriptWindow = !IsAddScriptWindow;
					IsCreateWindow = false;
				}
				ImGui::PopStyleColor(3);

				ImGui::Dummy(ImVec2(0.0f, 10.0f));

				ImGui::End();

				ImGui::SetNextWindowPos(ImVec2(ImGui::GetMainViewport()->Size.x - 250.0f, 0.0f), ImGuiCond_Always);
				ImGui::SetNextWindowSize(ImVec2(250.0f, ImGui::GetMainViewport()->Size.y - 250.0f), ImGuiCond_Always);
				ImGui::Begin("Properties Window", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration);

				ImGui::Text("Properties");

				ImGui::Dummy(ImVec2(0.0f, 10.0f));
				ImGui::Separator();
				ImGui::Dummy(ImVec2(0.0f, 10.0f));

				if (Target)
				{
					char NameBuff[64] = "";

					strncpy(NameBuff, Target->Get_Name().c_str(), sizeof(NameBuff) - 1);
					NameBuff[sizeof(NameBuff) - 1] = '\0';

					ImGui::Text("Name: %s", NameBuff);

					ImGui::Dummy(ImVec2(0.0f, 10.0f));
					ImGui::Separator();
					ImGui::Dummy(ImVec2(0.0f, 10.0f));

					ImGui::BeginGroup();

					ImGui::Text("Size");
					ImGui::Dummy(ImVec2(10.0f, 0.0f));
					ImGui::SameLine();
					ImGui::Text("X");
					ImGui::SameLine();
					float MaxSizeX = SceneWidth;
					ImGui::DragFloat("##SizeX", &Target->Get_Size().X, 1, 0, MaxSizeX, "%.2f");

					ImGui::Dummy(ImVec2(10.0f, 0.0f));
					ImGui::SameLine();
					ImGui::Text("Y");
					ImGui::SameLine();
					float MaxSizeY = SceneHeight;
					ImGui::DragFloat("##SizeY", &Target->Get_Size().Y, 1, 0, MaxSizeY, "%.2f");

					ImGui::EndGroup();

					ImGui::Dummy(ImVec2(0.0f, 10.0f));
					ImGui::Separator();
					ImGui::Dummy(ImVec2(0.0f, 10.0f));

					ImGui::BeginGroup();

					ImGui::Text("Position");
					ImGui::Dummy(ImVec2(10.0f, 0.0f));
					ImGui::SameLine();
					ImGui::Text("X");
					ImGui::SameLine();
					float MaxPositionX = SceneWidth - Target->Get_Size().X;
					ImGui::DragFloat("##PositionX", &Target->Get_Position().X, 1, 0, MaxPositionX, "%.2f");

					ImGui::Dummy(ImVec2(10.0f, 0.0f));
					ImGui::SameLine();
					ImGui::Text("Y");
					ImGui::SameLine();
					float MaxPositionY = SceneHeight - Target->Get_Size().Y;
					ImGui::DragFloat("##PositionY", &Target->Get_Position().Y, 1, 0, MaxPositionY, "%.2f");

					ImGui::EndGroup();

					ImGui::Dummy(ImVec2(0.0f, 10.0f));
					ImGui::Separator();
					ImGui::Dummy(ImVec2(0.0f, 10.0f));

					ImGui::BeginGroup();

					ImGui::Text("Color");
					ImGui::Dummy(ImVec2(10.0f, 0.0f));
					ImGui::SameLine();
					ImGui::Text("Red");
					ImGui::SameLine();
					ImGui::DragInt("##EntityColorRed", &Red, 1.0f, 0, 255);

					ImGui::Dummy(ImVec2(10.0f, 0.0f));
					ImGui::SameLine();
					ImGui::Text("Green");
					ImGui::SameLine();
					ImGui::DragInt("##EntityColorGreen", &Green, 1.0f, 0, 255);

					ImGui::Dummy(ImVec2(10.0f, 0.0f));
					ImGui::SameLine();
					ImGui::Text("Blue");
					ImGui::SameLine();
					ImGui::DragInt("##EntityColorBlue", &Blue, 1.0f, 0, 255);

					Target->Get_Color().Red = Red;
					Target->Get_Color().Green = Green;
					Target->Get_Color().Blue = Blue;

					ImGui::EndGroup();

					ImGui::Dummy(ImVec2(0.0f, 10.0f));
					ImGui::Separator();
					ImGui::Dummy(ImVec2(0.0f, 10.0f));

					ImGui::BeginGroup();
					
					static char NewEntityImageFilepathBuff[128] = "";

					ImGui::Checkbox("Is Image", &Target->Get_Sprite().IsImage);

					if (Target->Get_Sprite().IsImage)
					{
						if (NewEntityImageFilepathBuff[0] == '\0')
						{
							strncpy(NewEntityImageFilepathBuff, Target->Get_Sprite().FilePath.c_str(), sizeof(NewEntityImageFilepathBuff));
						}

						ImGui::Dummy(ImVec2(10.0f, 0.0f));
						ImGui::SameLine();
						ImGui::Text("Path");
						ImGui::SameLine();
						ImGui::InputText("##CurEntityImageFilepath", NewEntityImageFilepathBuff, sizeof(NewEntityImageFilepathBuff));

						ImGui::Dummy(ImVec2(10.0f, 0.0f));
						ImGui::SameLine();
						ImGui::PushStyleColor(ImGuiCol_Button, ImGuiCol_ControlButtonColor);
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGuiCol_ControlButtonHoveredColor);
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImGuiCol_ControlButtonActiveColor);
						if (ImGui::Button("Apply"))
						{
							Target->Get_Sprite().FilePath = NewEntityImageFilepathBuff;
							SpriteComponent::Apply(&Target->Get_Sprite(), this->p_Renderer);
						}
						ImGui::PopStyleColor(3);
					}

					ImGui::EndGroup();

					ImGui::Dummy(ImVec2(0.0f, 10.0f));
					ImGui::Separator();
					ImGui::Dummy(ImVec2(0.0f, 10.0f));

					ImGui::BeginGroup();

					float DeleteButtonWidth = ImGui::GetWindowWidth() - 20.0f;

					ImGui::PushStyleColor(ImGuiCol_Button, ImGuiCol_ControlButtonColor);
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGuiCol_ControlButtonHoveredColor);
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImGuiCol_ControlButtonActiveColor);
					if (ImGui::Button("Destroy", ImVec2(DeleteButtonWidth, 20.0f)))
					{
						for (size_t i = 0; i < entities.size(); i++)
						{
							if (&entities[i] == Target)
							{
								entities.erase(entities.begin() + i);

								Target = nullptr;
								TargetIndex = -1;
							}
						}
					}
					ImGui::PopStyleColor(3);

					ImGui::EndGroup();
				}

				ImGui::End();

				ImGui::SetNextWindowPos(ImVec2(250.0f, 0.0f));
				ImGui::SetNextWindowSize(ImVec2(ImGui::GetMainViewport()->Size.x - 500.0f, ImGui::GetMainViewport()->Size.y - 250.0f), ImGuiCond_Always);
				ImGui::Begin("Preview Window", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration);

				float PreviewTitleDist = (ImGui::GetWindowSize().x / 2.0f) - (ImGui::CalcTextSize("Preview").x / 2.0f);
				ImGui::Dummy(ImVec2(PreviewTitleDist, 0.0f));
				ImGui::SameLine();
				ImGui::Text("Preview");

				if (ImGui::BeginChild("Preview Area", ImVec2(0, 0), true))
				{
					ImVec2 Region = ImGui::GetContentRegionAvail();
					ImVec2 CursorStartPos = ImGui::GetCursorPos();

					float Aspect = (float)SceneWidth / (float)SceneHeight;
					ImVec2 ImageSize = Region;

					if (Region.x / Region.y > Aspect)
					{
						ImageSize.x = Region.y * Aspect;
					}
					else
					{
						ImageSize.y = Region.x / Aspect;
					}

					float XOffset = (Region.x - ImageSize.x) * 0.5f;
					float YOffset = (Region.y - ImageSize.y) * 0.5f;

					sceneOffset.x = ImGui::GetWindowPos().x + ImGui::GetCursorPos().x + XOffset;
					sceneOffset.y = ImGui::GetWindowPos().y + ImGui::GetCursorPos().y + YOffset;

					ImGui::SetCursorPos(ImVec2(CursorStartPos.x + XOffset, CursorStartPos.y + YOffset));

					ImGui::Image(SceneId, ImageSize);

					ImVec2 MousePos = ImGui::GetMousePos();

					if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::IsKeyDown(ImGuiKey_LeftShift))
					{
						if (MousePos.x >= sceneOffset.x && MousePos.x <= sceneOffset.x + ImageSize.x &&
							MousePos.y >= sceneOffset.y && MousePos.y <= sceneOffset.y + ImageSize.y)
						{
							ImVec2 LocalMouse(MousePos.x - sceneOffset.x, MousePos.y - sceneOffset.y);
							float ScaleX = SceneWidth / ImageSize.x;
							float ScaleY = SceneHeight / ImageSize.y;

							ImVec2 TextureMouse(LocalMouse.x * ScaleX, LocalMouse.y * ScaleY);

							Target = nullptr;
							TargetIndex = -1;

							for (size_t i = 0; i < entities.size(); i++)
							{
								Entity& ent = entities[i];

								float ex = ent.Get_Position().X;
								float ey = ent.Get_Position().Y;
								float ew = ent.Get_Size().X;
								float eh = ent.Get_Size().Y;

								if (TextureMouse.x >= ex && TextureMouse.x <= ex + ew &&
									TextureMouse.y >= ey && TextureMouse.y <= ey + eh)
								{
									Target = &ent;
									TargetIndex = i;
									break;
								}
							}
						}
					}
					if (ImGui::IsKeyDown(ImGuiKey_LeftShift) && Target && ImGui::IsMouseClicked(0))
					{
						if (MousePos.x >= sceneOffset.x && MousePos.x <= sceneOffset.x + ImageSize.x &&
							MousePos.y >= sceneOffset.y && MousePos.y <= sceneOffset.y + ImageSize.y)
						{
							ImVec2 LocalMouse(MousePos.x - sceneOffset.x, MousePos.y - sceneOffset.y);
							float ScaleX = SceneWidth / ImageSize.x;
							float ScaleY = SceneHeight / ImageSize.y;

							ImVec2 TextureMouse(LocalMouse.x* ScaleX, LocalMouse.y* ScaleY);

							if (TextureMouse.x - (Target->Get_Size().X / 2) >= 0 && TextureMouse.x + Target->Get_Size().X <= SceneWidth && TextureMouse.y - Target->Get_Size().Y >= 0 && TextureMouse.y + Target->Get_Size().Y <= SceneHeight)
							{
								Target->Set_Position(Position(TextureMouse.x - (Target->Get_Size().X / 2), TextureMouse.y - (Target->Get_Size().Y / 2)));
							}
						}
					}
				}

				ImGui::EndChild();

				ImGui::End();

				ImGui::SetNextWindowPos(ImVec2(0, ImGui::GetMainViewport()->Size.y - 250.0f));
				ImGui::SetNextWindowSize(ImVec2(250.0f, 250.0f), ImGuiCond_Always);
				ImGui::Begin("Window Settings Window", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration);

				ImGui::Text("Window Settings");

				int BackgroundWidth = SceneWidth;
				int BackgroundHeight = SceneHeight;

				ImGui::Dummy(ImVec2(0.0f, 10.0f));
				ImGui::Separator();
				ImGui::Dummy(ImVec2(0.0f, 10.0f));

				ImGui::BeginGroup();

				ImGui::Text("Width");
				ImGui::SameLine();
				ImGui::Dummy(ImVec2(10.0f, 0.0f));
				ImGui::SameLine();
				ImGui::PushItemWidth(100.0f);
				ImGui::DragInt("##WindowWidth", &BackgroundWidth, 1, 500, 1920, "%d");
				ImGui::PopItemWidth();

				ImGui::Text("Height");
				ImGui::SameLine();
				ImGui::Dummy(ImVec2(10.0f, 0.0f));
				ImGui::SameLine();
				ImGui::PushItemWidth(100.0f);
				ImGui::DragInt("##WindowHeight", &BackgroundHeight, 1, 500, 1080, "%d");
				ImGui::PopItemWidth();

				if (SceneWidth != BackgroundWidth || SceneHeight != BackgroundHeight)
				{
					SDL_DestroyTexture(GameScene);
					GameScene = SDL_CreateTexture(this->p_Renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, BackgroundWidth, BackgroundHeight);
					SDL_GetTextureSize(GameScene, &SceneWidth, &SceneHeight);
				}

				ImGui::EndGroup();

				int BackgroundRed = (int)SceneBackgroundColors.Red;
				int BackgroundGreen = (int)SceneBackgroundColors.Green;
				int BackgroundBlue = (int)SceneBackgroundColors.Blue;

				ImGui::Dummy(ImVec2(0.0f, 5.0f));
				ImGui::Separator();
				ImGui::Dummy(ImVec2(0.0f, 5.0f));

				ImGui::BeginGroup();

				ImGui::Text("Color Red");
				ImGui::SameLine();
				ImGui::Dummy(ImVec2(5.0f, 0.0f));
				ImGui::SameLine();
				ImGui::PushItemWidth(100.0f);
				ImGui::DragInt("##WindowRed", &BackgroundRed, 1, 0, 255, "%d");
				ImGui::PopItemWidth();

				ImGui::Text("Color Green");
				ImGui::SameLine();
				ImGui::Dummy(ImVec2(5.0f, 0.0f));
				ImGui::SameLine();
				ImGui::PushItemWidth(100.0f);
				ImGui::DragInt("##WindowGreen", &BackgroundGreen, 1, 0, 255, "%d");
				ImGui::PopItemWidth();

				ImGui::Text("Color Blue");
				ImGui::SameLine();
				ImGui::Dummy(ImVec2(5.0f, 0.0f));
				ImGui::SameLine();
				ImGui::PushItemWidth(100.0f);
				ImGui::DragInt("##WindowBlue", &BackgroundBlue, 1, 0, 255, "%d");
				ImGui::PopItemWidth();

				if (SceneBackgroundColors.Red != (Uint32)BackgroundRed)
					SceneBackgroundColors.Red = (Uint32)BackgroundRed;

				if (SceneBackgroundColors.Green != (Uint32)BackgroundGreen)
					SceneBackgroundColors.Green = (Uint32)BackgroundGreen;

				if (SceneBackgroundColors.Blue != (Uint32)BackgroundBlue)
					SceneBackgroundColors.Blue = (Uint32)BackgroundBlue;

				ImGui::EndGroup();

				ImGui::End();

				ImGui::SetNextWindowPos(ImVec2(250.0f, ImGui::GetMainViewport()->Size.y - 250.0f));
				ImGui::SetNextWindowSize(ImVec2(ImGui::GetMainViewport()->Size.x, ImGui::GetMainViewport()->Size.y - 250.0f), ImGuiCond_Always);
				ImGui::Begin("Assets", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration);

				ImGui::Text("Assets");

				ImGui::Dummy(ImVec2(10.0f, 10.0f));

				ImGui::TextColored(ImVec4(0.9f, 0.1f, 0.1f, 1.0f), "I don't know what to put here. scripts should have been here but eh, I am too lazy to move it.");

				ImGui::End();

				if (IsCreateWindow)
				{
					ImGui::SetNextWindowSize(ImVec2(500.0f, 375.0f), ImGuiCond_Once);
					ImGui::Begin("Create Object Window", &IsCreateWindow, ImGuiWindowFlags_NoDecoration);

					ImGui::Text("Create Object");

					static int NewEntityId = 0;
					static char NewEntityNameBuff[64] = "";

					static float NewEntityXPosition = 0.0f;
					static float NewEntityYPosition = 0.0f;

					static float NewEntityXSize = 0.0f;
					static float NewEntityYSize = 0.0f;

					static Uint32 NewEntityRed = 0, NewEntityGreen = 0, NewEntityBlue = 0;

					ImGui::Text("Name");
					ImGui::SameLine();
					ImGui::Dummy(ImVec2(5.0f, 0.0f));
					ImGui::SameLine();
					ImGui::InputText("##NewEntityName", NewEntityNameBuff, sizeof(NewEntityNameBuff));

					ImGui::Text("Id");
					ImGui::SameLine();
					ImGui::Dummy(ImVec2(5.0f, 0.0f));
					ImGui::SameLine();
					ImGui::InputInt("##NewEntityId", &NewEntityId, 1, 10);

					if (NewEntityId < 1)
					{
						NewEntityId = 1;
					}

					ImGui::Text("Size");
					ImGui::Text("X");
					ImGui::SameLine();
					ImGui::Dummy(ImVec2(5.0f, 0.0f));
					ImGui::SameLine();
					ImGui::DragFloat("##NewEntityXSize", &NewEntityXSize, 1.0f, 0.0f, SceneWidth);

					ImGui::Text("Y");
					ImGui::SameLine();
					ImGui::Dummy(ImVec2(5.0f, 0.0f));
					ImGui::SameLine();
					ImGui::DragFloat("##NewEntityYSize", &NewEntityYSize, 1.0f, 0.0f, SceneHeight);

					ImGui::Text("Position");
					ImGui::Text("X");
					ImGui::SameLine();
					ImGui::Dummy(ImVec2(5.0f, 0.0f));
					ImGui::SameLine();
					static float MaxNewEntityXPos = SceneWidth - NewEntityXPosition;
					ImGui::DragFloat("##NewEntityXPos", &NewEntityXPosition, 1.0f, 0.0f, MaxNewEntityXPos);

					ImGui::Text("Y");
					ImGui::SameLine();
					ImGui::Dummy(ImVec2(5.0f, 0.0f));
					ImGui::SameLine();
					static float MaxNewEntityYPos = SceneHeight - NewEntityYPosition;
					ImGui::DragFloat("##NewEntityYPos", &NewEntityYPosition, 1.0f, 0.0f, MaxNewEntityYPos);

					static int TempRed = 0;
					ImGui::Text("Color");
					ImGui::Text("Red");
					ImGui::SameLine();
					ImGui::Dummy(ImVec2(5.0f, 0.0f));
					ImGui::SameLine();
					ImGui::DragInt("##NewEntityRed", &TempRed, 1, 0, 255);
					NewEntityRed = (Uint32)TempRed;

					static int TempGreen = 0;
					ImGui::Text("Green");
					ImGui::SameLine();
					ImGui::Dummy(ImVec2(5.0f, 0.0f));
					ImGui::SameLine();
					ImGui::DragInt("##NewEntityGreen", &TempGreen, 1, 0, 255);
					NewEntityGreen = (Uint32)TempGreen;

					static int TempBlue = 0;
					ImGui::Text("Blue");
					ImGui::SameLine();
					ImGui::Dummy(ImVec2(5.0f, 0.0f));
					ImGui::SameLine();
					ImGui::DragInt("##NewEntityBlue", &TempBlue, 1, 0, 255);
					NewEntityBlue = (Uint32)TempBlue;

					static char NewEntityImageFilepathBuff[128] = "";
					static bool NewEntityIsImage = false;

					ImGui::Checkbox("Is Image", &NewEntityIsImage);

					if (NewEntityIsImage)
					{
						ImGui::Text("Image Filepath");
						ImGui::SameLine();
						ImGui::Dummy(ImVec2(5.0f, 0.0f));
						ImGui::SameLine();
						ImGui::InputText("##NewEntityImageFilepath", NewEntityImageFilepathBuff, sizeof(NewEntityImageFilepathBuff));
					}
					else
					{
						NewEntityImageFilepathBuff[0] = '\0';
					}

					static bool isCreateError = false;

					float CreateButtonSize = ImGui::GetWindowWidth() - 20.0f;

					ImGui::PushStyleColor(ImGuiCol_Button, ImGuiCol_ControlButtonColor);
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGuiCol_ControlButtonHoveredColor);
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImGuiCol_ControlButtonActiveColor);
					if (ImGui::Button("Create", ImVec2(CreateButtonSize, 20.0f)))
					{
						if (strlen(NewEntityNameBuff) < 1)
						{
							isCreateError = true;
						}
						else
						{
							bool isNameUsed = false;
							bool isIdUsed = false;

							for (Entity& entity : entities)
							{
								if (entity.Get_Name() == std::string(NewEntityNameBuff))
								{
									isNameUsed = true;
									break;
								}

								if (entity.Get_Id() == NewEntityId)
								{
									isIdUsed = true;
									break;
								}
							}

							if (isNameUsed)
							{
								isCreateError = true;
							}
							else if (isIdUsed)
							{
								isCreateError = true;
							}
							else
							{
								isCreateError = false;

								Entity NewEntity(NewEntityId, std::string(NewEntityNameBuff), Position(NewEntityXPosition, NewEntityYPosition), Size(NewEntityXSize, NewEntityYSize), Color(NewEntityRed, NewEntityGreen, NewEntityBlue, (Uint32)255), SpriteComponent(NewEntityIsImage, std::string(NewEntityImageFilepathBuff)));

								if (NewEntityIsImage)
								{
									SpriteComponent::Apply(&NewEntity.Get_Sprite(), this->p_Renderer);
								}
								else
								{
									NewEntity.Get_Sprite().Texture = nullptr;
								}

								entities.push_back(NewEntity);
								Target = nullptr;
								TargetIndex = -1;

								NewEntityId = 0;

								NewEntityXPosition = 0.0f;
								NewEntityYPosition = 0.0f;

								NewEntityXSize = 0.0f;
								NewEntityYSize = 0.0f;

								NewEntityRed = 0, NewEntityGreen = 0, NewEntityBlue = 0;
								TempRed = 0;
								TempGreen = 0;
								TempBlue = 0;

								NewEntityIsImage = false;
								NewEntityNameBuff[0] = '\0';
								NewEntityImageFilepathBuff[0] = '\0';

								IsCreateWindow = false;
							}
						}
					}
					ImGui::PopStyleColor(3);

					ImGui::End();
				}

				if (IsAddScriptWindow)
				{
					ImGui::SetNextWindowSize(ImVec2(500.0f, 100.0f), ImGuiCond_Once);
					ImGui::Begin("Add Lua Script", &IsAddScriptWindow, ImGuiWindowFlags_NoDecoration);

					ImGui::Text("Lua Script");

					static char ScriptBuff[64] = "";

					ImGui::Text("Current Script: %s", strlen(ScriptBuff) > 0 ? ScriptBuff : "N/A");

					ImGui::Text("Filepath");
					ImGui::SameLine();
					ImGui::Dummy(ImVec2(5.0f, 0.0f));
					ImGui::SameLine();
					ImGui::InputText("##IntegratedScript", ScriptBuff, sizeof(ScriptBuff));

					float IntegrateButtonSize = ImGui::GetWindowWidth() - 20.0f;

					ImGui::PushStyleColor(ImGuiCol_Button, ImGuiCol_ControlButtonColor);
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGuiCol_ControlButtonHoveredColor);
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImGuiCol_ControlButtonActiveColor);
					if (ImGui::Button("Integrate", ImVec2(IntegrateButtonSize, 20.0f)))
					{
						IntegratedScriptFilepath = ScriptBuff;

						IsAddScriptWindow = false;
					}
					ImGui::PopStyleColor(3);

					ImGui::End();
				}

				ImGui::PopFont();

				ImGui::Render();

				SDL_SetRenderDrawColor(this->p_Renderer, 0, 0, 0, 0);
				SDL_RenderClear(this->p_Renderer);

				SDL_SetRenderTarget(this->p_Renderer, GameScene);
				SDL_Rect ViewPort = { 0, 0, SceneWidth, SceneHeight };
				SDL_SetRenderViewport(this->p_Renderer, &ViewPort);

				SDL_SetRenderScale(this->p_Renderer, 1.0f, 1.0f);

				SDL_SetRenderDrawColor(this->p_Renderer, SceneBackgroundColors.Red, SceneBackgroundColors.Green, SceneBackgroundColors.Blue, 255);
				SDL_RenderClear(this->p_Renderer);

				for (Entity& entity : entities)
				{
					if (&entity == Target)
					{
						SDL_SetRenderDrawColor(this->p_Renderer, 255, 215, 0, 255);

						SDL_FRect brect = {
							static_cast<float>(entity.Get_Position().X - 2),
							static_cast<float>(entity.Get_Position().Y - 2),
							static_cast<float>(entity.Get_Size().X + 4),
							static_cast<float>(entity.Get_Size().Y + 4)
						};

						SDL_RenderFillRect(this->p_Renderer, &brect);
					}

					SDL_SetRenderDrawColor(this->p_Renderer, entity.Get_Color().Red, entity.Get_Color().Green, entity.Get_Color().Blue, 255);
					SDL_FRect rect = {
						static_cast<float>(entity.Get_Position().X),
						static_cast<float>(entity.Get_Position().Y),
						static_cast<float>(entity.Get_Size().X),
						static_cast<float>(entity.Get_Size().Y)
					};

					if (entity.Get_Sprite().IsImage)
					{
						if (entity.Get_Sprite().Texture)
						{
							SDL_RenderTexture(this->p_Renderer, entity.Get_Sprite().Texture, nullptr, &rect);
						}
						else
						{
							SDL_RenderFillRect(this->p_Renderer, &rect);
						}
					}
					else
					{
						SDL_RenderFillRect(this->p_Renderer, &rect);
					}
				}

				SDL_SetRenderTarget(this->p_Renderer, nullptr);
				int WindowW, WindowH;
				SDL_GetWindowSize(this->p_Window, &WindowW, &WindowH);

				SDL_Rect WindowViewPort = { 0, 0, WindowW, WindowH };
				SDL_SetRenderViewport(this->p_Renderer, &WindowViewPort);
				SDL_SetRenderScale(this->p_Renderer, 1.0f, 1.0f);

				if (ImGui::GetDrawData())
				{
					ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), this->p_Renderer);
				}

				SDL_RenderPresent(this->p_Renderer);
			}

			Target = nullptr;

			WindowSize windowSize;
			SDL_GetTextureSize(GameScene, &windowSize.X, &windowSize.Y);

			this->apply(windowSize, SceneBackgroundColors, IntegratedScriptFilepath, entities);

			std::ifstream scriptCheck;
			scriptCheck.open(IntegratedScriptFilepath);

			if (!scriptCheck.good())
			{
				scriptCheck.close();

				std::filesystem::path sfilepath = IntegratedScriptFilepath;

				if (sfilepath.has_parent_path())
				{
					std::error_code ec;
					std::filesystem::create_directories(sfilepath.parent_path(), ec);
				}

				std::ofstream scriptApply(IntegratedScriptFilepath);

				if (scriptApply.is_open())
				{
					scriptApply << "function Start()\n\t\nend\n\nfunction Loop()\n\t\nend\n";
					scriptApply.close();
				}
			}
			else
			{
				scriptCheck.close();
			}

			std::ofstream outputFile;
			outputFile.open(filepath, std::ios::out);

			if (outputFile.is_open())
			{

				for (std::string line : this->p_Data)
				{
					outputFile << line << '\n';
				}

				outputFile.close();

				std::vector<std::string> games = {};

				std::ifstream i_GamesFile;
				i_GamesFile.open("games.txt");

				if (i_GamesFile.is_open())
				{
					std::string curLine = "";

					while (std::getline(i_GamesFile, curLine))
					{
						bool is_in = false;

						for (const std::string& cur : games)
						{
							if (curLine == cur)
							{
								is_in = true;
							}
						}

						if (!is_in)
						{
							games.push_back(curLine);
						}
					}

					i_GamesFile.close();

					games.push_back(this->p_Filepath);

					std::ofstream o_GamesFile;
					o_GamesFile.open("games.txt", std::ios::out | std::ios::trunc);

					if (o_GamesFile.is_open())
					{
						for (const std::string& gamePath : games)
						{
							o_GamesFile << gamePath << '\n';
						}

						o_GamesFile.close();
					}
				}

				entities.clear();
			}

			SDL_DestroyTexture(GameScene);

			ImGui_ImplSDLRenderer3_Shutdown();
			ImGui_ImplSDL3_Shutdown();
			ImGui::DestroyContext();

			SDL_DestroyRenderer(this->p_Renderer);
			SDL_DestroyWindow(this->p_Window);
		}
		else
		{
			SDL_DestroyWindow(this->p_Window);
		}
	}
	else
	{
		std::cout << "[CPP]: Failed to initialize editor!\n";
	}
}

void VX::Designer::apply(const WindowSize& windowSize, const Color& windowColor, const std::string& scriptFilepath, std::vector<Entity>& entities)
{
	this->p_Data.clear();

	this->p_Data.push_back(std::to_string(windowSize.X) + "," + std::to_string(windowSize.Y) + "," + std::to_string(windowColor.Red) + "," + std::to_string(windowColor.Green) + "," + std::to_string(windowColor.Blue));
	this->p_Data.push_back("$/EOWDTED/$");

	for (Entity& entity : entities)
	{
		const std::string entityName = entity.Get_Name();
		const int entityId = entity.Get_Id();
		const Position& entityPos = entity.Get_Position();
		const Size& entitySize = entity.Get_Size();
		const Color& entityColor = entity.Get_Color();
		const SpriteComponent& spriteComponent = entity.Get_Sprite();

		int isImage = spriteComponent.IsImage ? 1 : 0;

		this->p_Data.push_back(entityName + "," + std::to_string(entityId) + "," + std::to_string(entitySize.X) + "," + std::to_string(entitySize.Y) + "," + std::to_string(entityPos.X) + "," + std::to_string(entityPos.Y) + "," + std::to_string(entityColor.Red) + "," + std::to_string(entityColor.Green) + "," + std::to_string(entityColor.Blue) + "," + std::to_string(isImage) + "," + spriteComponent.GetPath());
	}

	this->p_Data.push_back("$/EOEDTSFP/$");
	this->p_Data.push_back(scriptFilepath);
	this->p_Data.push_back("$/EOGD/$");
}