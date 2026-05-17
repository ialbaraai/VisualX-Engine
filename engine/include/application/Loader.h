#pragma once

#include <iostream>
#include "../system/Backend.h"

namespace VX
{
	class Loader
	{
	private:
		bool is_target_sdl;
		float p_TimeTaken = 0.0f;
	public:
		Loader(const bool& TargetIsSDL) : is_target_sdl(TargetIsSDL) {}

		float Get_TimeTaken() const;
		void Set_TimeTaken(const float& newTime);
		
		bool Get_IsSDL() const;

		bool run();

		~Loader() = default;
	};
}