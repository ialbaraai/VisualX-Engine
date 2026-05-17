#ifndef BACKEND_H

#define BACKEND_H

#include <iostream>
#include <string>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>

template <typename... Types>
void VX_Log(Types&&... args)
{
	std::cout << "[VX LOG]: ";
	(
		( std::cout << args << " "),
		...
	);
	std::cout << '\n';
}

#endif