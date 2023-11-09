#include <SDL.h>

#include <vector>
#include <iostream>
#include <chrono>

#include "game.h"
#include "renderer.h"
#include "profiler.h"

Game::Game(SDL_Window* window, int SCREEN_WIDTH, int SCREEN_HEIGHT):
	window(window), SCREEN_WIDTH(SCREEN_WIDTH), SCREEN_HEIGHT(SCREEN_HEIGHT) 
{
	setupRenderers();

	float VIEW_SCALE = 1.2f;
	
	this->solver = Solver();
	quit = false;
}

static float rot = 0.0f;

static Profiler pr("Profiler", 100);

void Game::startLoop() {
	
	std::vector<float> positions;
	while (!quit) {
		pr.start();

		handleEvents();

		float dt = pr.duration * 0.000000001f;
		solver.solve(dt);

		positions = solver.getPositions();
		circleRenderer.setInstanceData(&positions);
		circleRenderer.render(true);

		float width = 100.f;
		float height = 50.0f;

		//rot += 0.2;

		std::vector<float> rectPositions =
		{
			0.0f, -(height/2) - .5f, width, 1.0f, cos(rot), sin(rot), 1.0f, 1.0f, 1.0f, 1.0f,
			0.0f, (height/2) + .5f, width, 1.0f, cos(rot), sin(rot), 1.0f, 1.0f, 1.0f, 0.0f,
			-(width/2) - .5f, 0.0f, 1.0f, height, cos(rot), sin(rot), 1.0f, 1.0f, 1.0f, 0.0f,
			(width/2) + .5f, 0.0f, 1.0f, height, cos(rot), sin(rot), 1.0f, 1.0f, 1.0f, 1.0f
		};
		rectangleRenderer.setInstanceData(&rectPositions);
		rectangleRenderer.render(false);

		SDL_GL_SwapWindow(window);
		pr.end();
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

			//solver.addParticle(Vec2((x - xOffset) * (2 * VIEW_WIDTH / SCREEN_WIDTH), -((y - yOffset) * (2 * VIEW_HEIGHT / SCREEN_HEIGHT))));

			for (float dx = -10; dx <= 10; dx += 2)
				for (float dy = -10; dy <= 10; dy += 2)
					solver.addParticle(Vec2((x - xOffset) * (2*circleRenderer.VIEW_WIDTH/SCREEN_WIDTH) + dx, -((y - yOffset) * (2*circleRenderer.VIEW_HEIGHT/SCREEN_HEIGHT)) + dy),
						Vec2(0.0f, 0.0f));
		}

		/*
		if (e.type == SDL_MOUSEMOTION)
		{
			int x, y;
			SDL_GetMouseState(&x, &y);
			float yOffset = (float)(SCREEN_HEIGHT / 2);
			float xOffset = (float)(SCREEN_WIDTH / 2);
			solver.addParticle(Vec2((x - xOffset) * (2*VIEW_WIDTH/SCREEN_WIDTH), -((y - yOffset) * (2*VIEW_HEIGHT/SCREEN_HEIGHT))));
		}
		*/
		
	}
}