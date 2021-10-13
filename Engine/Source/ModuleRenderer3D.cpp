#include "Application.h"
#include "ModuleRenderer3D.h"

#include "ModuleWindow.h"
#include "ModuleCamera3D.h"
#include "ModuleEditor.h"
#include "glew/include/GL/glew.h"
#include "SDL\include\SDL_opengl.h"


#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_sdl.h"
#include "Imgui/imgui_impl_opengl3.h"

#include "Optick/include/optick.h"

#include "mmgr/mmgr.h"

ModuleRenderer3D::ModuleRenderer3D(bool startEnabled) : Module(startEnabled)
{
	name = "Renderer";
	context = NULL;

	depthTest = true;
	cullFace = true;
	colorMaterial = true;
	lighting = true;
	texture2D = true;
	stencil = false;
	blending = false;

	wireMode = false;
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init(JsonParsing& node)
{
	LOG("Creating 3D Renderer context");
	bool ret = true;
	
	//Create context
	context = SDL_GL_CreateContext(app->window->window);
	if(context == NULL)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	
	if(ret == true)
	{
		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			/* Problem: glewInit failed, something is seriously wrong. */
			LOG("Error: %s\n", glewGetErrorString(err));
		}

		LOG("Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

		//Use Vsync
		if(VSYNC && SDL_GL_SetSwapInterval(1) < 0)
			LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		ImGui::StyleColorsDark();
		
		ImGui_ImplSDL2_InitForOpenGL(app->window->window, context);
		ImGui_ImplOpenGL3_Init();

		LOG("Vendor: %s", glGetString(GL_VENDOR));
		LOG("Renderer: %s", glGetString(GL_RENDERER));
		LOG("OpenGL version supported %s", glGetString(GL_VERSION));
		LOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			ret = false;
		}
		
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		
		//Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			ret = false;
		}
		
		GLfloat lightModelAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lightModelAmbient);
		
		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();
		
		GLfloat materialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, materialAmbient);

		GLfloat materialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, materialDiffuse);
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		lights[0].Active(true);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_TEXTURE_2D);
	}

	// Uncomment for using framebuffer
	//glGenFramebuffers(1, &framebuffer);
	//glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	//unsigned int textureColorbuffer;
	//glGenTextures(1, &textureColorbuffer);
	//glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1080, 720, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glBindTexture(GL_TEXTURE_2D, 0);

	//// attach it to currently bound framebuffer object
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
	//
	//unsigned int rbo;
	//glGenRenderbuffers(1, &rbo);
	//glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
	//glBindRenderbuffer(GL_RENDERBUFFER, 0);

	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	//glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//// Projection matrix for
	OnResize(*app->window->GetWindowWidth(), *app->window->GetWindowHeight());

	//cubeId = 0;
	//glGenBuffers(1, &cubeId);

	//index = 0;
	//glGenBuffers(1, &index);

	//PCube* cube = new PCube({0,0,0}, {0,0,0}, {1,1,1});
	//primitives.push_back(cube);

	//PPyramid* pyramid = new PPyramid({ 2,0,0 }, { 0,0,0 }, { 1,1,1 });
	//primitives.push_back(pyramid);
	//
	//PCylinder* cyl = new PCylinder(50, 2.0f, 1.0f);
	//primitives.push_back(cyl);
	//
	//PSphere* sphere = new PSphere(1.0f, 20, 20);
	//primitives.push_back(sphere);

	//InitMesh();
	
	return ret;
}

// PreUpdate: clear buffer
bool ModuleRenderer3D::PreUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(app->camera->GetViewMatrix());

	// light 0 on cam pos
	lights[0].SetPos(app->camera->position.x, app->camera->position.y, app->camera->position.z);

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	return true;
}

bool ModuleRenderer3D::Update(float dt)
{
	// Uncomment for direct mode Opengl Cube
	//DrawCubeDirectMode();

	// Uncomment for Vertex Array
	//GLfloat vertices[108] =
	//{
	//	// Front
	//	1.0f, 1.0f, 0.0f,
	//	0.0f, 1.0f, 0.0f,
	//	0.0f, 0.0f, 0.0f,

	//	0.0f, 0.0f, 0.0f,
	//	1.0f, 0.0f, 0.0f,
	//	1.0f, 1.0f, 0.0f,

	//	// Right
	//	1.0f, 1.0f, 0.0f,
	//	1.0f, 0.0f, 0.0f,
	//	1.0f, 0.0f,-1.0f,

	//	1.0f, 0.0f,-1.0f,
	//	1.0f, 1.0f,-1.0f,
	//	1.0f, 1.0f, 0.0f,

	//	// top
	//	1.0f, 1.0f, 0.0f,
	//	1.0f, 1.0f,-1.0f,
	//	0.0f, 1.0f,-1.0f,

	//	0.0f, 1.0f,-1.0f,
	//	0.0f, 1.0f, 0.0f,
	//	1.0f, 1.0f, 0.0f,

	//	// Back
	//	0.0f, 0.0f,-1.0f,
	//	1.0f, 0.0f,-1.0f,
	//	1.0f, 0.0f, 0.0f,

	//	1.0f, 0.0f, 0.0f,
	//	0.0f, 0.0f, 0.0f,
	//	0.0f, 0.0f,-1.0f,

	//	// Left
	//	0.0f, 0.0f,-1.0f,
	//	0.0f, 0.0f, 0.0f,
	//	0.0f, 1.0f, 0.0f,

	//	0.0f, 1.0f, 0.0f,
	//	0.0f, 1.0f,-1.0f,
	//	0.0f, 0.0f,-1.0f,

	//	// Bottom
	//	0.0f, 0.0f,-1.0f,
	//	1.0f, 0.0f,-1.0f,
	//	1.0f, 0.0f, 0.0f,

	//	1.0f, 0.0f, 0.0f,
	//	0.0f, 0.0f, 0.0f,
	//	0.0f, 0.0f,-1.0f
	//};

	//
	//// Bind Buffer and store data
	//glBindBuffer(GL_ARRAY_BUFFER, cubeId);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 36 * 3, vertices, GL_STATIC_DRAW);

	//glEnableClientState(GL_VERTEX_ARRAY);

	//glBindBuffer(GL_ARRAY_BUFFER, cubeId);
	//glVertexPointer(3, GL_FLOAT, 0, NULL);
	//glDrawArrays(GL_TRIANGLES, 0, 36);

	//glDisableClientState(GL_VERTEX_ARRAY);

	return true;
}

// PostUpdate present buffer to screen
bool ModuleRenderer3D::PostUpdate()
{
	OPTICK_EVENT("Rendering");
	//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer);

	for (int i = 0; i < primitives.size(); ++i)
	{
		primitives[i]->Draw();
	}

	DrawMeshes();

	//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	
	app->editor->Draw();

	SDL_GL_SwapWindow(app->window->window);

	return true;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");

	for (int i = 0; i < primitives.size(); ++i)
	{
		RELEASE(primitives[i]);
	}

	glDeleteFramebuffers(1, &framebuffer);

	primitives.clear();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DeleteContext(context);

	return true;
}

bool ModuleRenderer3D::LoadConfig(JsonParsing& node)
{
	return true;
}

bool ModuleRenderer3D::SaveConfig(JsonParsing& node) const
{
	return true;
}

void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	projectionMatrix = Perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf(&projectionMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void ModuleRenderer3D::SetDepthTest()
{
	if (depthTest) glEnable(GL_DEPTH_TEST);
	else glDisable(GL_DEPTH_TEST);
}

void ModuleRenderer3D::SetCullFace()
{
	if (cullFace) glEnable(GL_CULL_FACE);
	else glDisable(GL_CULL_FACE);
}

void ModuleRenderer3D::SetLighting()
{
	if (lighting) glEnable(GL_LIGHTING);
	else glDisable(GL_LIGHTING);
}

void ModuleRenderer3D::SetColorMaterial()
{
	if (colorMaterial) glEnable(GL_COLOR_MATERIAL);
	else glDisable(GL_COLOR_MATERIAL);
}

void ModuleRenderer3D::SetTexture2D()
{
	if (texture2D) glEnable(GL_TEXTURE_2D);
	else glDisable(GL_TEXTURE_2D);
}

void ModuleRenderer3D::SetStencil()
{
	if (stencil) glEnable(GL_STENCIL_TEST);
	else glDisable(GL_STENCIL_TEST);
}

void ModuleRenderer3D::SetBlending()
{
	if (blending) glEnable(GL_BLEND);
	else glDisable(GL_BLEND);
}

void ModuleRenderer3D::SetWireMode()
{
	if (wireMode) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void ModuleRenderer3D::DrawCubeDirectMode()
{
	GLfloat v0[3] = { 1.0f, 1.0f, 0.0f };
	GLfloat v1[3] = { 0.0f, 1.0f, 0.0f };
	GLfloat v2[3] = { 0.0f, 0.0f, 0.0f };
	GLfloat v3[3] = { 1.0f, 0.0f, 0.0f };
	GLfloat v4[3] = { 1.0f, 0.0f,-1.0f };
	GLfloat v5[3] = { 1.0f, 1.0f,-1.0f };
	GLfloat v6[3] = { 0.0f, 1.0f,-1.0f };
	GLfloat v7[3] = { 0.0f, 0.0f,-1.0f };

	glBegin(GL_TRIANGLES);

	// Front
	glVertex3fv(v0);
	glVertex3fv(v1);
	glVertex3fv(v2);

	glVertex3fv(v2);
	glVertex3fv(v3);
	glVertex3fv(v0);

	// Right
	glVertex3fv(v0);
	glVertex3fv(v3);
	glVertex3fv(v4);

	glVertex3fv(v4);
	glVertex3fv(v5);
	glVertex3fv(v0);

	// Top
	glVertex3fv(v0);
	glVertex3fv(v5);
	glVertex3fv(v6);

	glVertex3fv(v6);
	glVertex3fv(v1);
	glVertex3fv(v0);

	// Back
	glVertex3fv(v7);
	glVertex3fv(v6);
	glVertex3fv(v5);

	glVertex3fv(v5);
	glVertex3fv(v4);
	glVertex3fv(v7);

	// Left
	glVertex3fv(v7);
	glVertex3fv(v2);
	glVertex3fv(v1);

	glVertex3fv(v1);
	glVertex3fv(v6);
	glVertex3fv(v7);

	// Bottom
	glVertex3fv(v7);
	glVertex3fv(v4);
	glVertex3fv(v3);

	glVertex3fv(v3);
	glVertex3fv(v2);
	glVertex3fv(v7);

	glEnd();
}

void ModuleRenderer3D::DrawMeshes()
{
	OPTICK_EVENT("Drawing all FBX");
	//DrawMesh();
	for (int i = 0; i < fbx.size(); ++i)
	{
		fbx[i].Draw();
	}
}

void ModuleRenderer3D::AddPrimitive(Primitive* primitive)
{
	primitives.push_back(primitive);
}

void ModuleRenderer3D::InitMesh(const char* filePath)
{
	OPTICK_EVENT("Loading FBX");
	fbx.push_back(Model(filePath));
	// TODO
	/*fbx.LoadMesh(filePath);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	fbxVertex = new VertexBuffer(fbx.GetMesh().vertices, fbx.GetMesh().numVertex * sizeof(float3));

	fbxIndex = new IndexBuffer(fbx.GetMesh().indices, fbx.GetMesh().numIndex);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float3), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float3), 0);
	glEnableVertexAttribArray(1);*/
}

void ModuleRenderer3D::DrawMesh()
{
	// TODO
	/*glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, fbx.GetMesh().numIndex, GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);*/
}