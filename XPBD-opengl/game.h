#pragma once

#include <SDL.h>

#include "renderer.h"
#include "xpbdSolver.h"
#include "scene.h"

class Game {
public:
	Game(SDL_Window* window, int SCREEN_WIDTH, int SCREEN_HEIGHT);

	void startLoop();
private:
	int SCREEN_WIDTH, SCREEN_HEIGHT;
	bool quit;

	SDL_Window* window;
	Renderer circleRenderer;
	Renderer rectangleRenderer;
	Solver* solver;
	Scene* scene;
		
	void setupRenderers();
	void handleEvents();
};
