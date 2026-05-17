#include "Loader.h"

float VX::Loader::Get_TimeTaken() const
{
	return this->p_TimeTaken;
}
void VX::Loader::Set_TimeTaken(const float& newTime)
{
	this->p_TimeTaken = newTime;
}
bool VX::Loader::Get_IsSDL() const
{
	return this->is_target_sdl;
}

VX::Loader* VXLoader_Init(const bool& IsSDL)
{
	return new VX::Loader(IsSDL);
}

bool run(VX::Loader* Loader)
{
	if (Loader->Get_IsSDL())
	{
		SDL_Window* window = SDL_CreateWindow("VX Loader", 400, 400, 0);

		if (!window)
		{
			return true;
		}

		SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);

		if (!renderer)
		{
			SDL_DestroyWindow(window);

			return true;
		}

		SDL_Surface* logoSurface = IMG_Load("VX Engine.jpeg");

		if (!logoSurface)
		{
			SDL_DestroyRenderer(renderer);
			SDL_DestroyWindow(window);

			return true;
		}

		SDL_SetWindowIcon(window, logoSurface);
	
		SDL_Texture* logoTexture = SDL_CreateTextureFromSurface(renderer, logoSurface);

		if (!logoTexture)
		{
			SDL_DestroySurface(logoSurface);
			SDL_DestroyRenderer(renderer);
			SDL_DestroyWindow(window);
			
			return true;
		}

		bool is_running = true;
		bool is_exit = false;

		Uint64 previousTick = SDL_GetTicks();
		float accumulator = 0.0f;

		SDL_FRect loadingBg = {50, 300, 300, 50};
		int loadingBgC[3] = { 25, 25, 25 };

		SDL_FRect loadingBarBg = { 55, 305, 0, 40 };
		int loadingBarBgC[3] = { 5, 200, 5 };

		SDL_FRect logoDestRect = { 25, 12.5, 350, 300 };

		SDL_Event event;

		while (is_running)
		{
			Uint64 currentTick = SDL_GetTicks();
			float deltaTime = (currentTick - previousTick) / 1000.0f;
			accumulator += deltaTime;

			Loader->Set_TimeTaken(Loader->Get_TimeTaken() + deltaTime);

			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_EVENT_QUIT)
				{
					is_running = false;
					is_exit = true;
				}
			}

			loadingBarBg.w = (Loader->Get_TimeTaken() / 4500.0f) * 290.0f;

			if (Loader->Get_TimeTaken() >= 4500.0f)
			{
				is_running = false;
			}

			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderClear(renderer);

			SDL_RenderTexture(renderer, logoTexture, nullptr, &logoDestRect);

			SDL_SetRenderDrawColor(renderer, loadingBgC[0], loadingBgC[1], loadingBgC[2], 255);
			SDL_RenderFillRect(renderer, &loadingBg);

			SDL_SetRenderDrawColor(renderer, loadingBarBgC[0], loadingBarBgC[1], loadingBarBgC[2], 255);
			SDL_RenderFillRect(renderer, &loadingBarBg);

			SDL_RenderPresent(renderer);
		}

		SDL_Delay(500);

		SDL_DestroyTexture(logoTexture);
		SDL_DestroySurface(logoSurface);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);

		return is_exit;
	}

	return false;
}

void end(VX::Loader*& Loader)
{
	if (Loader)
	{
		delete Loader;
	}
}