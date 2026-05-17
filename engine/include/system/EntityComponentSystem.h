#ifndef _ENTITY_COMPONENT_SYSTEM

#define _ENTITY_COMPONENT_SYSTEM

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "Backend.h"

#ifndef _WINDOW_THEME_TYPE

#define _WINDOW_THEME_TYPE

typedef uint32_t VX_Uint32;

constexpr VX_Uint32 WINDOW_THEME_DARK = 10001;
constexpr VX_Uint32 WINDOW_THEME_LIGHT = 20002;
constexpr VX_Uint32 WINDOW_THEME_NONE = 30003;

typedef float vxs;

enum class vxm
{
	Dark = WINDOW_THEME_DARK,
	Light = WINDOW_THEME_LIGHT,
	None = WINDOW_THEME_NONE
};

constexpr ImVec4 DARK_THEME_ImGuiCol_ControlButtonColor = { 0.20f, 0.22f, 0.26f, 1.0f };
constexpr ImVec4 DARK_THEME_ImGuiCol_ControlButtonHoveredColor = { 0.35f, 0.37f, 0.41f, 1.0f };
constexpr ImVec4 DARK_THEME_ImGuiCol_ControlButtonActiveColor = { 0.28f, 0.30f, 0.34f, 1.0f };
constexpr ImVec4 DARK_THEME_Background_Color = { 20.0f / 255.0f, 20.0f / 255.0f, 20.0f / 255.0f, 1.0f };

constexpr ImVec4 LIGHT_THEME_ImGuiCol_ControlButtonColor = { 0.85f, 0.85f, 0.85f, 1.0f };
constexpr ImVec4 LIGHT_THEME_ImGuiCol_ControlButtonHoveredColor = { 0.70f, 0.70f, 0.70f, 1.0f };
constexpr ImVec4 LIGHT_THEME_ImGuiCol_ControlButtonActiveColor = { 0.60f, 0.60f, 0.60f, 1.0f };

#endif


#ifndef _FPS

#define _FPS 60.0f

constexpr float FIXED_TIME_STEP = 1.0f / _FPS;
constexpr float FRAME_DELAY = 1000.0f / _FPS;

#endif

#ifndef _RUN_RESULT

#define _RUN_RESULT RunResult

enum class RunResult
{
	Exit,
	NextScene,
};

#endif

#ifndef _COMPONENT

#define _COMPONENT 1

class Component
{

};

#endif


#ifndef _POSITION

#define _POSITION 2

class Position : public Component
{
public:
	float X;
	float Y;

	Position()
	{
		this->X = 0;
		this->Y = 0;
	}

	Position(const int& x, const int& y)
	{
		this->X = x;
		this->Y = y;
	}
	Position(float x, float y)
	{
		this->X = x;
		this->Y = y;
	}
	Position(double x, double y)
	{
		this->X = x;
		this->Y = y;
	}
	Position(const short& x, const short& y)
	{
		this->X = x;
		this->Y = y;
	}
};

#endif

#ifndef _WINDOW_SIZE

#define _WINDOW_SIZE

class WindowSize : public Component
{
public:
	float X;
	float Y;

	WindowSize()
	{
		this->X = 0;
		this->Y = 0;
	}

	WindowSize(const float& x, const float& y)
	{
		this->X = x;
		this->Y = y;
	}
};

#endif

#ifndef _SIZE

#define _SIZE 3

class Size : public Component
{
public:
	float X;
	float Y;

	Size()
	{
		this->X = 0;
		this->Y = 0;
	}
	Size(const int& x, const int& y)
	{
		this->X = x;
		this->Y = y;
	}
	Size(float x, float y)
	{
		this->X = x;
		this->Y = y;
	}
	Size(double x, double y)
	{
		this->X = x;
		this->Y = y;
	}
	Size(const short& x, const short& y)
	{
		this->X = x;
		this->Y = y;
	}
};

#endif


#ifndef _VELOCITY_VECTOR

#define _VELOCITY_VECTOR 4

class Velocity : public Component
{
public:
	float DeltaX;
	float DeltaY;

	Velocity() = default;

	Velocity(const float& deltax, const float& deltay)
	{
		this->DeltaX = deltax;
		this->DeltaY = deltay;
	}
};

#endif

#ifndef _COLOR

#define _COLOR 9

class Color : public Component
{
public:
	Uint32 Red;
	Uint32 Green;
	Uint32 Blue;
	Uint32 Alpha;

	Color()
	{
		this->Red = 0;
		this->Green = 0;
		this->Blue = 0;
		this->Alpha = 255;
	}

	Color(const Uint32& red, const Uint32& green, const Uint32& blue, const Uint32& alpha)
	{
		this->Red = red;
		this->Green = green;
		this->Blue = blue;
		this->Alpha = alpha;
	}

	Color(const int& red, const int& green, const int& blue, const int& alpha)
	{
		this->Red = red;
		this->Green = green;
		this->Blue = blue;
		this->Alpha = alpha;
	}
	
	Color(float red, float green, float blue, float alpha)
	{
		this->Red = red;
		this->Green = green;
		this->Blue = blue;
		this->Alpha = alpha;
	}
};

#endif

#ifndef _RENDERABLE

#define _RENDERABLE 17

class SpriteComponent
{
public:
	bool IsImage = false;
	std::string FilePath = "";
	SDL_Texture* Texture;

	SpriteComponent(bool is_image, std::string filepath)
	{
		this->IsImage = is_image;
		this->FilePath = filepath;
		this->Texture = nullptr;

		if (this->IsImage)
		{
			SDL_Surface* surface = IMG_Load(this->FilePath.c_str());

			if (!surface)
			{
				this->IsImage = false;
				this->FilePath = "";
			}

			SDL_DestroySurface(surface);
		}
	}

	void SetPath(const std::string& new_path)
	{
		this->FilePath = new_path;
	}

	static void Apply(SpriteComponent* component, SDL_Renderer* renderer)
	{
		if (component->IsImage)
		{
			if (component->Texture)
			{
				SDL_DestroyTexture(component->Texture);
			}

			component->Texture = IMG_LoadTexture(renderer, component->FilePath.c_str());

			if (!component->Texture)
			{
				std::cout << "Failed to load texture!\n";
				component->IsImage = false;
			}
		}
	}

	const std::string& GetPath() const
	{
		if (this->IsImage)
		{
			return this->FilePath;
		}
		else
		{
			return "";
		}
	}
};

#endif

#ifndef _VECTOR_2

#define _VECTOR_2 18

struct Vector2
{
	float X = 0.0f;
	float Y = 0.0f;

	Vector2() = default;
	Vector2(float x) : X(x) {}
	Vector2(float x, float y)
	{
		this->X = x;
		this->Y = y;
	}

	inline Vector2 operator+(Vector2 A)
	{
		return {this->X + A.X, this->Y + A.Y};
	}
	inline Vector2 operator-(Vector2 A)
	{
		return { this->X - A.X, this->Y - A.Y };
	}
	inline Vector2 operator*(float S)
	{
		return { this->X * S, this->Y * S };
	}
};

#endif

#endif