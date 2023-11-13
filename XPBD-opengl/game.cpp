#include <SDL.h>

#include "imgui.h"
#include "./backends/imgui_impl_sdl2.h"
#include "./backends/imgui_impl_opengl3.h"

#include <vector>
#include <iostream>
#include <chrono>

#include "game.h"
#include "renderer.h"
#include "profiler.h"
#include "xpbdBallScene.h"
#include "verletScene.h"

Game::Game(SDL_Window* window, int SCREEN_WIDTH, int SCREEN_HEIGHT):
	window(window), SCREEN_WIDTH(SCREEN_WIDTH), SCREEN_HEIGHT(SCREEN_HEIGHT) 
{
	setupRenderers();

	float VIEW_SCALE = 1.2f;
	
	this->scene = new XpbdBallScene();
	//this->scene = new VerletScene();

	quit = false;
}

static Profiler pr("Profiler", 100);

void Game::startLoop() {

	while (!quit) {
		pr.start();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();

		float dt = pr.duration * 0.000000001f;
		scene->step();
		scene->solver->solve(dt);
		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::End();

		handleEvents();

		std::vector<InstanceData> positions = scene->getCircleData();
		circleRenderer.setInstanceData(&positions);
		circleRenderer.render(true);

		std::vector<InstanceData> rectPositions = scene->getRectData();
		std::vector<LineData> lines = scene->getLineData();
		for (LineData ld : lines)
			rectPositions.push_back(ld.convertToInstanceData());
		rectangleRenderer.setInstanceData(&rectPositions);
		rectangleRenderer.render(false);

		ImGui::Render();

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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
		ImGui_ImplSDL2_ProcessEvent(&e);

		if (e.type == SDL_QUIT)
			quit = true;
		if (e.type == SDL_MOUSEBUTTONDOWN)
		{
			int x, y;
			SDL_GetMouseState(&x, &y);
			float yOffset = (float)(SCREEN_HEIGHT / 2);
			float xOffset = (float)(SCREEN_WIDTH / 2);
			Vec2 viewportPos = Vec2((x - xOffset) * (2 * circleRenderer.VIEW_WIDTH / SCREEN_WIDTH), -((y - yOffset) * (2 * circleRenderer.VIEW_HEIGHT / SCREEN_HEIGHT)));

			scene->mouseDownEvent(viewportPos);
		}
	}
}