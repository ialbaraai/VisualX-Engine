#include "../../include/application/Game.h"

/*Entity* Game::FindEntityById(int id)
{
	for (Entity* entity : this->p_Entities)
	{
		if (entity->Get_Id() == id && entity->Get_IsAlive())
		{
			return entity;
		}
	}

	return nullptr;
}*/

Game::Game(const std::string& WindowTitle, const WindowSize& size, const Color& color, const std::vector<Entity*>& entities, const std::string& script)
{
	this->p_GameName = WindowTitle;
	this->p_WindowTitle = "VX Launcher - " + WindowTitle;

	this->p_WindowSize = new WindowSize(size.X, size.Y);
	this->p_WindowColor = new Color(color.Red, color.Green, color.Blue, color.Alpha);

	this->p_LuaManager = new VX::LuaManager(script);
	this->p_RunTime = 0.0f;

	this->p_FPS = 60.0;
	this->p_FrameDuration = 1000.0 / this->p_FPS;

	this->p_LuaManager->LuaState.set_function("VX_ThirtyFPS", []() -> float { return 30.0f; });
	this->p_LuaManager->LuaState.set_function("VX_FourtyEightFPS", []() -> float { return 48.0f; });
	this->p_LuaManager->LuaState.set_function("VX_SixtyFPS", []() -> float { return 60.0f; });
	this->p_LuaManager->LuaState.set_function("VX_OneTwentyFPS", []() -> float { return 120.0f; });
	this->p_LuaManager->LuaState.set_function("VX_OneFourtyFourFPS", []() -> float { return 144.0f; });
	this->p_LuaManager->LuaState.set_function("VX_TwoFourtyFPS", []() -> float { return 240.0f; });

	this->p_LuaManager->LuaState.set_function("VX_SetFPS", [this](float FPS) {
		
		this->p_FPS = std::clamp((double)FPS, 0.0, 360.0);
		this->p_FrameDuration = 1000.0 / this->p_FPS;
		
		});

	sol::usertype<Position> position_type = this->p_LuaManager->LuaState.new_usertype<Position>("Position",
		sol::constructors<Position(), Position(float, float)>(),
		"X", &Position::X,
		"Y", &Position::Y
	);

	sol::usertype<Size> size_type = this->p_LuaManager->LuaState.new_usertype<Size>("Size",
		sol::constructors<Size(), Size(float, float)>(),
		"X", &Size::X,
		"Y", &Size::Y
	);

	sol::usertype<Color> color_type = this->p_LuaManager->LuaState.new_usertype<Color>("Color",
		sol::constructors<Color(), Color(float, float, float, float)>(),
		"Red", &Color::Red,
		"Green", &Color::Green,
		"Blue", &Color::Blue,
		"Alpha", &Color::Alpha
	);

	sol::usertype<SpriteComponent> spriteComponent_type = this->p_LuaManager->LuaState.new_usertype<SpriteComponent>("SpriteComponent",
		sol::constructors<SpriteComponent(bool, std::string)>(),
		"IsImage", &SpriteComponent::IsImage,
		"GetPath", &SpriteComponent::GetPath
	);

	this->p_LuaManager->LuaState.new_usertype<EntityHandler>("Entity",
		"Name", sol::property(&EntityHandler::Get_Name, &EntityHandler::Set_Name),
		"Id", sol::property(&EntityHandler::Get_Id),
		"IsAlive", sol::property(&EntityHandler::Get_IsAlive, &EntityHandler::Set_IsAlive),
		"Position", sol::property(
			[](EntityHandler& h) -> Position& { return h.p_Pointer->Get_Position(); },
			[](EntityHandler& h, const Position& p) { h.p_Pointer->Set_Position(p); }
		),
		"Size", sol::property(
			[](EntityHandler& h) -> Size& { return h.p_Pointer->Get_Size(); },
			[](EntityHandler& h, const Size& s) { h.p_Pointer->Set_Size(s); }
		),
		"Color", sol::property(
			[](EntityHandler& h) -> Color& { return h.p_Pointer->Get_Color(); },
			[](EntityHandler& h, const Color& c) { h.p_Pointer->Set_Color(c); }
		),
		"SpriteComponent", sol::property(
			[](EntityHandler& h) -> SpriteComponent& { return h.p_Pointer->Get_Sprite(); },
			[](EntityHandler& h, const SpriteComponent& s) { h.p_Pointer->Set_Sprite(s); }
		)
	);

	this->p_LuaManager->LuaState["WindowWidth"] = this->p_WindowSize->X;
	this->p_LuaManager->LuaState["WindowHeight"] = this->p_WindowSize->Y;
	
	sol::table keysTable = this->p_LuaManager->LuaState.create_table();

	sol::table entitiesTable = this->p_LuaManager->LuaState.create_table();
	this->p_LuaManager->LuaState["Entities"] = entitiesTable;

	sol::table mouseTable = this->p_LuaManager->LuaState.create_table();
	mouseTable["X"] = 0;
	mouseTable["Y"] = 0;
	this->p_LuaManager->LuaState["Mouse"] = mouseTable;

	for (char c = 'a'; c <= 'z'; ++c)
	{
		keysTable[std::string(1, c)] = SDL_Keycode(c);
	}

	for (char c = 'A'; c <= 'Z'; ++c)
	{
		keysTable[std::string(1, c)] = SDL_Keycode(c);
	}

	for (char c = '0'; c <= '9'; ++c)
	{
		keysTable[std::string(1, c)] = SDL_Keycode(c);
	}

	keysTable["ESC"] = SDLK_ESCAPE;
	keysTable["Space"] = SDLK_SPACE;
	keysTable["LeftShift"] = SDLK_LSHIFT;
	keysTable["RightShift"] = SDLK_RSHIFT;
	keysTable["LeftControl"] = SDLK_LCTRL;
	keysTable["RightControl"] = SDLK_RCTRL;

	keysTable["ArrowUp"] = SDLK_UP;
	keysTable["ArrowLeft"] = SDLK_LEFT;
	keysTable["ArrowDown"] = SDLK_DOWN;
	keysTable["ArrowRight"] = SDLK_RIGHT;

	this->p_LuaManager->LuaState["Keys"] = keysTable;

	this->p_LuaManager->LuaState.set_function("VX_CreateEntity", [this](int Id, const std::string& Name, const Position& Position, const Size& Size, const Color& Color, const SpriteComponent& SpriteComponent) {

		std::shared_ptr<EntityHandler> handle = this->p_EntitiesHandler.Create_Entity(Id, Name, Position, Size, Color, SpriteComponent);

		SpriteComponent::Apply(&handle->p_Pointer->Get_Sprite(), this->p_Renderer);
		
		this->p_LuaManager->LuaState["Entities"][Name] = handle;

		return handle;
		});
	this->p_LuaManager->LuaState.set_function("VX_FindEntityById", [this](int Id) -> sol::optional<std::shared_ptr<EntityHandler>> {
		std::shared_ptr<EntityHandler> handle = this->p_EntitiesHandler.FindEntityById(Id);

		if (handle && handle->p_Pointer->Get_IsAlive())
		{
			return handle;
		}

		return sol::nullopt;
		});

	this->p_LuaManager->LuaState.set_function("VX_IsMouseLeftDown", [this]() -> bool {return this->p_IsMouseLeftDown; });
	this->p_LuaManager->LuaState.set_function("VX_IsMouseLeftUp", [this]() -> bool {return this->p_IsMouseLeftUp; });
	this->p_LuaManager->LuaState.set_function("VX_IsMouseLeftHeld", [this]() -> bool {return this->p_IsMouseLeftHeld; });
	this->p_LuaManager->LuaState.set_function("VX_IsMouseRightDown", [this]() -> bool {return this->p_IsMouseRightDown; });
	this->p_LuaManager->LuaState.set_function("VX_IsMouseRightUp", [this]() -> bool {return this->p_IsMouseRightUp; });
	this->p_LuaManager->LuaState.set_function("VX_IsMouseRightHeld", [this]() -> bool {return this->p_IsMouseRightHeld; });
	this->p_LuaManager->LuaState.set_function("VX_IsMouseMiddleDown", [this]() -> bool {return this->p_IsMouseMiddleDown; });
	this->p_LuaManager->LuaState.set_function("VX_IsMouseMiddleUp", [this]() -> bool {return this->p_IsMouseMiddleUp; });
	this->p_LuaManager->LuaState.set_function("VX_IsMouseMiddleHeld", [this]() -> bool {return this->p_IsMouseMiddleHeld; });

	this->p_LuaManager->LuaState.set_function("VX_GetKeyDown", [this](int KEY) {
		return this->p_PreviousKeys.count(static_cast<SDL_Keycode>(KEY)) == 0 && this->p_CurrentKeys.count(static_cast<SDL_Keycode>(KEY)) != 0;
		});
	this->p_LuaManager->LuaState.set_function("VX_GetKeyUp", [this](int KEY) {
		return this->p_PreviousKeys.count(static_cast<SDL_Keycode>(KEY)) != 0 && this->p_CurrentKeys.count(static_cast<SDL_Keycode>(KEY)) == 0;
		});
	this->p_LuaManager->LuaState.set_function("VX_GetKeyHeld", [this](int KEY) {
		return this->p_PreviousKeys.count(static_cast<SDL_Keycode>(KEY)) != 0 && this->p_CurrentKeys.count(static_cast<SDL_Keycode>(KEY)) != 0;
		});

	this->p_LuaManager->LuaState.set_function("VX_GetRuntime", [this]() {
		return this->p_RunTime;
		});
	this->p_LuaManager->LuaState.set_function("VX_Wait", [this](float duration) {
		SDL_Delay(duration * 1000);
		});

	this->p_LuaManager->LuaState.set_function("VX_Quit", [this]() {this->p_IsRunning = false; });

	this->p_Window = SDL_CreateWindow(this->p_WindowTitle.c_str(), this->p_WindowSize->X, this->p_WindowSize->Y, 0);
	
	if (!this->p_Window)
	{
		std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << '\n';
		return;
	}

	this->p_Renderer = SDL_CreateRenderer(this->p_Window, nullptr); // SDL3 CreateRenderer takes a Window* and a const char*

	if (!this->p_Renderer)
	{
		std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << '\n';
		return;
	}

	this->p_LuaManager->LuaState.set_function("VX_SetImage", [this](int Id, const std::string& NewImage) -> bool {
		std::shared_ptr<EntityHandler> handle = this->p_EntitiesHandler.FindEntityById(Id);
		
		if (handle && handle->p_Pointer->Get_IsAlive())
		{
			if (NewImage == handle->p_Pointer->Get_Sprite().FilePath)
			{
				return true;
			}

			const bool& prevIsImage = handle->Get_Sprite().IsImage;
			const std::string prevImg = handle->Get_Sprite().GetPath();

			handle->Set_Sprite(SpriteComponent(true, NewImage));

			SpriteComponent::Apply(&handle->p_Pointer->Get_Sprite(), this->p_Renderer);

			if (handle->Get_Sprite().Texture)
			{
				return true;
			}
			else
			{
				handle->Set_Sprite(SpriteComponent(prevIsImage, prevImg));
				return false;
			}
		}
		});

	this->p_LuaManager->LuaState.set_function("VX_SetThumbnail", [this](const std::string& NewThumbnail) -> bool {

		SDL_Surface* thumbnail = IMG_Load(NewThumbnail.c_str());

		if (thumbnail)
		{
			if (SDL_SetWindowIcon(this->p_Window, thumbnail))
			{
				SDL_DestroySurface(thumbnail);

				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
		});

	for (Entity* entity : entities)
	{
		if (!this->p_EntitiesHandler.FindEntityById(entity->Get_Id()))
		{
			std::shared_ptr<EntityHandler> handle = this->p_EntitiesHandler.Create_Entity(entity->Get_Id(), entity->Get_Name(), entity->Get_Position(), entity->Get_Size(), entity->Get_Color(), entity->Get_Sprite());
			SpriteComponent::Apply(&handle->p_Pointer->Get_Sprite(), this->p_Renderer);

			this->p_LuaManager->LuaState["Entities"][entity->Get_Name()] = handle;
			// this->p_Entities.push_back(entity);
		}
	}

	try
	{
		if (this->p_LuaManager->LoadScript())
		{
			this->p_IsRunning = true;
		}
	}
	catch (const std::exception& e)
	{
		SDL_DestroyRenderer(this->p_Renderer);
		SDL_DestroyWindow(this->p_Window);

		std::cerr << "This Talk Not Walk Error: Failed to load script: " << script << '\n';
	}
}
Game::~Game()
{
	/*for (Entity* entity : this->p_Entities)
	{
		this->p_LuaManager->LuaState[entity->Get_Name()] = sol::nil;
	}*/

	if (this->p_LuaManager)
	{
		sol::table ents = this->p_LuaManager->LuaState["Entities"];
		ents.clear();
	}

	/*for (Entity* entity : this->p_Entities)
	{
		delete entity;
	}*/

	this->p_EntitiesHandler.Cleanup();

	VXLua_Destroy(this->p_LuaManager);

	delete this->p_WindowSize;
	delete this->p_WindowColor;

	SDL_DestroyRenderer(this->p_Renderer);
	SDL_DestroyWindow(this->p_Window);
}

Game* Game_Init(const std::string& WindowTitle, const WindowSize& size, const Color& color, const std::vector<Entity*>& entities, const std::string& script)
{
	Game* game = new Game(WindowTitle, size, color, entities, script);

	return game;
}

void Game::HandleInput(const SDL_Event& event)
{
	if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
	{
		this->p_IsRunning = false;
	}
	if (event.type == SDL_EVENT_KEY_DOWN)
	{
		this->p_CurrentKeys.insert(event.key.key);
	}
	if (event.type == SDL_EVENT_KEY_UP)
	{
		this->p_CurrentKeys.erase(event.key.key);
	}

	if (event.type == SDL_EVENT_MOUSE_MOTION)
	{
		this->p_MouseX = event.motion.x;
		this->p_MouseY = event.motion.y;

		this->p_LuaManager->LuaState["Mouse"]["X"] = this->p_MouseX;
		this->p_LuaManager->LuaState["Mouse"]["Y"] = this->p_MouseY;
	}

	if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
	{
		if (event.button.button == SDL_BUTTON_LEFT)
		{
			this->p_IsMouseLeftDown = true;
		}
		if (event.button.button == SDL_BUTTON_RIGHT)
		{
			this->p_IsMouseRightDown = true;
		}
		if (event.button.button == SDL_BUTTON_MIDDLE)
		{
			this->p_IsMouseMiddleDown = true;
		}
	}
	if (event.type == SDL_EVENT_MOUSE_BUTTON_UP)
	{
		if (event.button.button == SDL_BUTTON_LEFT)
		{
			this->p_IsMouseLeftUp = true;
			this->p_IsMouseLeftHeld = false;
			this->p_IsMouseLeftDown = false;
		}
		if (event.button.button == SDL_BUTTON_RIGHT)
		{
			this->p_IsMouseRightUp = true;
			this->p_IsMouseRightHeld = false;
			this->p_IsMouseRightDown = false;
		}
		if (event.button.button == SDL_BUTTON_MIDDLE)
		{
			this->p_IsMouseMiddleUp = true;
			this->p_IsMouseMiddleHeld = false;
			this->p_IsMouseMiddleDown = false;
		}
	}
}


void Game::HandleRender()
{
	SDL_SetRenderDrawColor(this->p_Renderer, this->p_WindowColor->Red, this->p_WindowColor->Green, this->p_WindowColor->Blue, this->p_WindowColor->Alpha);
	SDL_RenderClear(this->p_Renderer);

	for (std::shared_ptr<EntityHandler>& handle : this->p_EntitiesHandler.entities)
	{
		if (!handle) continue;
		if (!handle->p_Pointer) continue;
		if (!handle->Get_IsAlive()) continue;
		//if (entity->Get_Position().X > this->p_WindowSize->X) continue;
		//if (entity->Get_Position().Y > this->p_WindowSize->Y) continue;

		SDL_SetRenderDrawColor(this->p_Renderer, handle->Get_Color().Red, handle->Get_Color().Green, handle->Get_Color().Blue, handle->Get_Color().Alpha);
		SDL_FRect entityRectangle = { handle->Get_Position().X, handle->Get_Position().Y, handle->Get_Size().X, handle->Get_Size().Y };

		if (handle->Get_Sprite().IsImage)
		{
			SDL_RenderTexture(this->p_Renderer, handle->Get_Sprite().Texture, nullptr, &entityRectangle);
		}
		else
		{
			SDL_RenderFillRect(this->p_Renderer, &entityRectangle);
		}
	}

	SDL_RenderPresent(this->p_Renderer);
}

void GameLoop(Game* game)
{
	std::cout << "-------------------------------------------------------------------------" << '\n';

	std::cout << "Game Launching: " << game->p_GameName << '\n';

	SDL_Event event;
	
	Uint64 previousTick = SDL_GetTicks();
	float accumulator = 0.0f;
	
	sol::protected_function start = game->p_LuaManager->LuaState["Start"];

	if (start.valid())
	{
		auto result = start();

		if (!result.valid())
		{
			sol::error err = result;
			std::cerr << "Lua Start Error: " << err.what() << '\n';
			return;
		}
	}

	while (game->p_IsRunning)
	{
		Uint64 currentTick = SDL_GetTicks();
		float deltaTime = (currentTick - previousTick) / 1000.0f;
		accumulator += deltaTime;

		game->p_RunTime += deltaTime;

		while (SDL_PollEvent(&event))
		{
			ImGui_ImplSDL3_ProcessEvent(&event);

			game->HandleInput(event);
		}

		while (accumulator >= FIXED_TIME_STEP)
		{
			sol::protected_function loop = game->p_LuaManager->LuaState["Loop"];

			if (loop.valid())
			{
				auto result = loop(FIXED_TIME_STEP);

				if (!result.valid())
				{
					sol::error err = result;
					std::cerr << "Lua Loop Error: " << err.what() << '\n';
					return;
				}
			}

			game->p_PreviousKeys = game->p_CurrentKeys;

			accumulator -= FIXED_TIME_STEP;
		}

		game->HandleRender();

		previousTick = SDL_GetTicks();

		Uint64 duration = SDL_GetTicks() - currentTick;


		if (duration < static_cast<Uint64>(game->p_FrameDuration))
		{
			SDL_Delay(static_cast<Uint64>(game->p_FrameDuration - duration));
		}
	}

	std::cout << "-------------------------------------------------------------------------" << '\n';
}

RunResult EndGame(Game* game)
{
	delete game;

	return RunResult::Exit;
}