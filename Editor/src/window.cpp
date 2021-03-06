#include "window.h"
#include <iostream>
#include "glad/glad.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"
#include "helper.h"

ImFont* Window::openSansBold = NULL;
ImFont* Window::openSansLight = NULL;

int Window::width = 800;
int Window::height = 600;

Window::Window(Input* _input) {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		std::cout << "sdl gave error" << std::endl;
		return;
	}

	// set opengl attributes through sdl
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	uint32_t windowFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
	window = SDL_CreateWindow("window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Window::width, Window::height, windowFlags);

	context = SDL_GL_CreateContext(window);

	SDL_GL_MakeCurrent(window, context);
	gladLoadGLLoader(SDL_GL_GetProcAddress);

	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	// set up ImGUi
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ioPtr = &io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	ImGui_ImplSDL2_InitForOpenGL(window, context);
	const char* glslVersion = "#version 330";
	ImGui_ImplOpenGL3_Init(glslVersion);

	// create fonts
	float fontSize = 16.0f;

	// get font paths
	char fontPath[200] = {};
	Helper::GetApplicationPath(fontPath);
	Helper::ConcatBuffer(fontPath, "\\assets\\fonts\\OpenSans-Bold.ttf");
	Window::openSansBold = io.Fonts->AddFontFromFileTTF(fontPath, fontSize);

	const char* slash = "\\";
	int slashIdx = Helper::GetLastIndex(fontPath, *slash);
	*(fontPath + slashIdx + 1) = 0;
	Helper::ConcatBuffer(fontPath, "OpenSans-Light.ttf");
	Window::openSansBold = io.Fonts->AddFontFromFileTTF(fontPath, fontSize);

	slashIdx = Helper::GetLastIndex(fontPath, *slash);
	*(fontPath + slashIdx + 1) = 0;
	Helper::ConcatBuffer(fontPath, "OpenSans-Regular.ttf");
	io.FontDefault = io.Fonts->AddFontFromFileTTF(fontPath, fontSize);

	input = _input;
}

void Window::pollEvents() {

	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		input->enterPressed = false;
		ImGui_ImplSDL2_ProcessEvent(&event);
		if (event.type == SDL_QUIT) {
			running = false;
		}
		if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window)) {
			running = false;
		}
		if (event.type == SDL_KEYDOWN) {
			SDL_Keycode keyDown = event.key.keysym.sym;
			input->enterPressed = (keyDown == SDLK_RETURN);
		}
	}
}

void Window::initGuiForFrame() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
}

void Window::swapBuffers() {
	if (ioPtr->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		// handles the multiple windows created by ImGui
		SDL_Window* backupCurrentWindow = SDL_GL_GetCurrentWindow();
		SDL_GLContext backupCurrentContext = SDL_GL_GetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		SDL_GL_MakeCurrent(backupCurrentWindow, backupCurrentContext);
	}
	SDL_GL_SwapWindow(window);
}

glm::vec2 Window::getWindowSize() {
	int size[2];
	SDL_GetWindowSize(window, &size[0], &size[1]);
	return glm::vec2(size[0], size[1]);
}