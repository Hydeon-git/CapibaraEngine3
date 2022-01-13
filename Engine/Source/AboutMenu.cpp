#include "AboutMenu.h"

#include <SDL_syswm.h>
#include <GL\glew.h>

#include "assimp/version.h"
#include "DevIL/include/IL/il.h"
#include "PhysFS/include/physfs.h"

#include "Profiling.h"

AboutMenu::AboutMenu() : Menu(false)
{
}

AboutMenu::~AboutMenu()
{
}

bool AboutMenu::Update(float dt)
{
	if (ImGui::Begin("About Capibara Engine", &active))
	{
		ImGui::Separator();
		ImGui::Text("Capibara Engine\n");
		ImGui::Text("\nDeveloped for Videogame Engines Class in CITM-UPC\n");
		ImGui::Text("\nBy Albert Pou, Arnau Bonada and Pol Pallares\n");
		ImGui::Separator();

		ImGui::TextWrapped("3rd Party Libraries used:");
		SDL_version sdlVer;
		SDL_GetVersion(&sdlVer);
		ImGui::TextWrapped("-SDL %d.%d.%d", sdlVer.major, sdlVer.minor, sdlVer.patch);
		ImGui::TextWrapped("-Glew %s", glewGetString(GLEW_VERSION));
		ImGui::TextWrapped("-ImGui %s", ImGui::GetVersion());
		ImGui::TextWrapped("-MathGeoLib 1.5");
		ImGui::TextWrapped("-OpenGl version %s", glGetString(GL_VERSION));
		ImGui::TextWrapped("-Assimp version %d.%d", aiGetVersionMajor(), aiGetVersionMinor());
		ImGui::TextWrapped("-DevIL version 1.8.0");
		ImGui::TextWrapped("-PhysFS version %d.%d.%d", PHYSFS_VER_MAJOR, PHYSFS_VER_MINOR, PHYSFS_VER_PATCH);

		ImGui::Separator();
		ImGui::Text("LICENSE\n");
		ImGui::Separator();

		ImGui::Text("MIT License\n\n");
		ImGui::Text("Copyright (c) 2021 CapibaraEngine\n\n");
		ImGui::Text("Permission is hereby granted, free of charge, to any person obtaining a copy\n\nof this software and associated documentation files (the 'Software'), to deal\n");
		ImGui::Text("in the Software without restriction, including without limitation the rights\n\nto use, copy, modify, merge, publish, distribute, sublicense, and/or sell\n");
		ImGui::Text("copies of the Software, and to permit persons to whom the Software is\n\nfurnished to do so, subject to the following conditions : \n");
		ImGui::Text("\n");
		ImGui::Text("The above copyright notice and this permission notice shall be included in all\n\ncopies or substantial portions of the Software.\n");
		ImGui::Text("\n");
		ImGui::Text("THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\nIMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, \n");
		ImGui::Text("FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE\nAUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\n");
		ImGui::Text("LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\nOUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\n");
		ImGui::Text("SOFTWARE.\n");
	}
	ImGui::End();

	return true;
}