#pragma once

#include <SDL.h>

#include "renderer.h"
#include "solver.h"

class Game {
public:
	Game(SDL_Window* window, int SCREEN_WIDTH, int SCREEN_HEIGHT);

	void startLoop();
private:
	float VIEW_WIDTH, VIEW_HEIGHT;
	int SCREEN_WIDTH, SCREEN_HEIGHT;
	bool quit;

	SDL_Window* window;
	Renderer circleRenderer;
	Renderer rectangleRenderer;
	Solver solver;
		
	void setupRenderers();
	void handleEvents();
};
