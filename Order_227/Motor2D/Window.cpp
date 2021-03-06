#include "Defs.h"
#include "Log.h"
#include "App.h"
#include "Window.h"
#include "Point.h"

#include "SDL/include/SDL.h"

Window::Window() : Module()
{
	name.assign("window");
	window = nullptr;
	screen_surface = nullptr;
}

// Destructor
Window::~Window()
{
}

// Called before render is available
bool Window::Awake(pugi::xml_node& config)
{
	LOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		Uint32 flags = SDL_WINDOW_SHOWN;
		bool fullscreen = config.child("fullscreen").attribute("value").as_bool(false);
		bool borderless = config.child("borderless").attribute("value").as_bool(false);
		bool resizable = config.child("resizable").attribute("value").as_bool(false);
		bool fullscreen_window = config.child("fullscreen_window").attribute("value").as_bool(false);

		width = middle_x = config.child("resolution").attribute("width").as_int(640);
		height = middle_y = config.child("resolution").attribute("height").as_int(480);
		scale = config.child("resolution").attribute("scale").as_int(1);

		middle_x = (uint)(middle_x * 0.5f);
		middle_y = (uint)(middle_y * 0.5f);

		if(fullscreen == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if(borderless == true)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if(resizable == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if(fullscreen_window == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		const char*tit = myApp->GetTitle();
		window = SDL_CreateWindow(tit, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if(window == nullptr)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

// Called before quitting
bool Window::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != nullptr)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

// Set new window title
void Window::SetTitle(const char* new_title)
{
	//title.create(new_title);
	SDL_SetWindowTitle(window, new_title);
}

void Window::GetWindowSize(uint& width, uint& height) const
{
	width = this->width;
	height = this->height;
}

void Window::GetWindowCenter(uint& mid_x, uint& mid_y) const
{
	mid_x = this->middle_x;
	mid_y = this->middle_y;
}

uint Window::GetScale() const
{
	return scale;
}