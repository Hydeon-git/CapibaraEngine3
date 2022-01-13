#pragma once
#include <windows.h>
#include <stdio.h>

#define DEBUG_LOG(format, ...) Log(__FILE__, __LINE__, format, __VA_ARGS__)

void Log(const char file[], int line, const char* format, ...);

#define CAP(n) ((n <= 0.0f) ? n=0.0f : (n >= 1.0f) ? n=1.0f : n=n)

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#define HAVE_M_PI

// Deletes a buffer
#define RELEASE( x )\
    {\
       if( x != nullptr )\
       {\
         delete x;\
	     x = nullptr;\
       }\
    }

// Folders defines
#define ASSETS_FOLDER "Assets/"
#define RESOURCES_FOLDER "Assets/Resources/"
#define TEXTURES_FOLDER "Assets/Resources/Textures/"
#define MODELS_FOLDER "Assets/Resources/Models/"
#define SCRIPTS_FOLDER "Assets/Resources/Scripts/"
#define SCENES_FOLDER "Assets/Scenes/"
#define SETTINGS_FOLDER "Settings/"
#define LIBRARY_FOLDER "Library/"
#define LIBRARY_MODELS_FOLDER "Library/Models/"
#define LIBRARY_MESHES_FOLDER "Library/Meshes/"
#define LIBRARY_SCRIPTS_FOLDER "Library/Scripts/"
#define LIBRARY_TEXTURES_FOLDER "Library/Textures/"
#define MATERIALS_FOLDER "Library/Materials/"

// Deletes an array of buffers
#define RELEASE_ARRAY( x )\
	{\
       if( x != nullptr )\
       {\
           delete[] x;\
	       x = nullptr;\
		 }\
	 }

typedef unsigned int uint;

// Configuration -----------
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#define SCREEN_SIZE 1
#define WIN_FULLSCREEN false
#define WIN_RESIZABLE true
#define WIN_BORDERLESS false
#define WIN_FULLSCREEN_DESKTOP false
#define VSYNC true
#define TITLE "Capibara Engine"