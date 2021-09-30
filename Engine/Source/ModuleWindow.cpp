#include "Application.h"

#include "ModuleWindow.h"

ModuleWindow::ModuleWindow(bool startEnabled) : Module(startEnabled)
{
	name = "Window";
	window = NULL;
	screenSurface = NULL;

	fullscreen = false;
	resizable = false;
	borderless = true;
	fullscreenDesktop = false;

	width = 0;
	height = 0;
	refreshRate = 0;

	brightness = 0;
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init(JsonParsing& node)
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
		width = (int)node.GetJsonNumber("width") * SCREEN_SIZE;
		height = (int)node.GetJsonNumber("height") * SCREEN_SIZE;
		
		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

		//Use OpenGL 2.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		if(WIN_FULLSCREEN == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if(WIN_RESIZABLE == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if(WIN_BORDERLESS == true)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if(WIN_FULLSCREEN_DESKTOP == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if(window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screenSurface = SDL_GetWindowSurface(window);
			brightness = SDL_GetWindowBrightness(window);
			SDL_DisplayMode* display = new SDL_DisplayMode();
			SDL_GetWindowDisplayMode(window, display);
			refreshRate = display->refresh_rate;
			RELEASE(display);
		}
	}

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

bool ModuleWindow::SaveConfig(JsonParsing& node) const
{
	node.SetNewJsonNumber(node.ValueToObject(node.GetRootValue()), "width", width);
	node.SetNewJsonNumber(node.ValueToObject(node.GetRootValue()), "height", height);
	return true;
}

void ModuleWindow::SetTitle(const char* title) const
{
	SDL_SetWindowTitle(window, title);
}

void ModuleWindow::SetFullscreen() const
{
	SDL_SetWindowFullscreen(window, fullscreen);
}

void ModuleWindow::SetFullscreenDesktop() const
{
	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
}

void ModuleWindow::SetResizable() const
{
}

void ModuleWindow::SetBorderless() const
{
	SDL_SetWindowBordered(window, (SDL_bool)borderless);
}

void ModuleWindow::SetBrightness() const
{
	SDL_SetWindowBrightness(window, brightness);
}

void ModuleWindow::SetWindowSize() const
{
	SDL_SetWindowSize(window, width, height);
}

int* ModuleWindow::GetWindowWidth()
{
	SDL_GetWindowSize(window, &width, &height);
	return &width;
}

int* ModuleWindow::GetWindowHeight()
{
	SDL_GetWindowSize(window, &width, &height);
	return &height;
}