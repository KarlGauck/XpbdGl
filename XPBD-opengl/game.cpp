#include <SDL.h>

#include <vector>
#include <iostream>
#include <chrono>

#include "game.h"
#include "renderer.h"

Game::Game(SDL_Window* window, int SCREEN_WIDTH, int SCREEN_HEIGHT):
	window(window), SCREEN_WIDTH(SCREEN_WIDTH), SCREEN_HEIGHT(SCREEN_HEIGHT) 
{
	setupRenderers();

	float VIEW_SCALE = 1.2f;
	VIEW_WIDTH = VIEW_SCALE * 16.f;
	VIEW_HEIGHT = VIEW_SCALE * 9.f;
	
	this->solver = Solver();
	quit = false;
}

void Game::startLoop() {
	
	std::vector<float> positions;
	while (!quit) {
		auto start = std::chrono::high_resolution_clock::now();

		handleEvents();

		solver.solve();

		positions = solver.getPositions();
		circleRenderer.setInstanceData(&positions);
		circleRenderer.render(true);

		std::vector<float> rectPositions =
		{
			0.0f, -8.5f, 32.0f, 1.0f, 0.0f,
			0.0f, 8.5f, 32.0f, 1.0f, 0.0f,
			-15.5f, 0.0f, 1.0f, 18.0f, 0.0f,
			15.5f, 0.0f, 1.0f, 18.0f, 0.0f
		};
		rectangleRenderer.setInstanceData(&rectPositions);
		rectangleRenderer.render(false);

		SDL_GL_SwapWindow(window);

		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
		std::cout << 1000.f/duration.count() << " FPS" << std::endl;
	}
}

void Game::setupRenderers()
{
	// Circle Renderer
	int fanCount = 50;
	std::vector<float> vertexData;
	vertexData.push_back(0.0f);
	vertexData.push_back(0.0f);

	for (int i = 0; i < fanCount; i++) {
		float angle = (float)(2 * M_PI / fanCount) * i;
		float x = cos(angle) / 2;
		float y = sin(angle) / 2;
		vertexData.push_back(x);
		vertexData.push_back(y);
	}

	std::vector<unsigned int> indexData;
	for (int i = 0; i < fanCount; i++) {
		std::cout << 0 << "  :  " << ((i % fanCount) + 1) << "  :  " << ((i + 1) % fanCount) + 1 << std::endl;
		indexData.push_back(0);
		indexData.push_back((i%fanCount) + 1);
		indexData.push_back(((i+1)%fanCount) + 1);
	}

	this->circleRenderer = Renderer(vertexData, indexData, "vertexShader.glsl", "fragmentShader.glsl");

	// Rectangle Renderer
	vertexData =
	{
		-0.5f, 0.5f,
		-0.5f, -0.5f,
		0.5f, -0.5f,
		0.5f, 0.5f
	};
	indexData = 
	{
		0, 1, 2,
		0, 2, 3
	};
	this->rectangleRenderer = Renderer(vertexData, indexData, "vertexShader.glsl", "fragmentShader.glsl");

}

void Game::handleEvents() {
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0) {
		if (e.type == SDL_QUIT)
			quit = true;
		if (e.type == SDL_MOUSEBUTTONDOWN)
		{
			int x, y;
			SDL_GetMouseState(&x, &y);
			float yOffset = (float)(SCREEN_HEIGHT / 2);
			float xOffset = (float)(SCREEN_WIDTH / 2);
			solver.addParticle(Vec2((x - xOffset) * (2*VIEW_WIDTH/SCREEN_WIDTH), -((y - yOffset) * (2*VIEW_HEIGHT/SCREEN_HEIGHT))));
		}
	}
}