#define GLEW_STATIC

#include <iostream>

#include <SDL.h>
#include <glew.h>

#include "imgui.h"
#include "./backends/imgui_impl_sdl2.h"
#include "./backends/imgui_impl_opengl3.h"

#include "game.h"

const int SCREEN_WIDTH = 160 * 7;
const int SCREEN_HEIGHT = 90 * 7; 
SDL_Window* window;
SDL_GLContext context;

bool init() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "SDL could not be initialized: " << SDL_GetError() << std::endl;
		return false;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	window = SDL_CreateWindow("SDL Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

	if (window == NULL) {
		std::cout << "Window could not be created: " << SDL_GetError() << std::endl;
		return false;
	}

	context = SDL_GL_CreateContext(window);
	if (context == NULL) {
		std::cout << "GL Context could not be created: " << SDL_GetError() << std::endl;
		return false;
	}

	glewExperimental = GL_TRUE;
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK) {
		std::cout << "Error initializing glew: " << glewGetErrorString(glewError) << std::endl;
		return false;
	}

	if (SDL_GL_SetSwapInterval(1) < 0) {
		std::cout << "Unable to set VSync: " << SDL_GetError() << std::endl;
	}


	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	ImGui_ImplSDL2_InitForOpenGL(window, context);
	ImGui_ImplOpenGL3_Init("#version 330");

	return true;
}

int main(int argc, char* args[]) {
	std::cout << "Hello World" << std::endl;
	if (!init())
		return -1;

	Game game(window, SCREEN_WIDTH, SCREEN_HEIGHT);
	game.startLoop();

	ImGui::DestroyContext();

	return 0;
}