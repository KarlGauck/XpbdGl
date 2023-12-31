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

		handleEvents();

		std::vector<InstanceData> positions = scene->getCircleData();
		circleRenderer.setInstanceData(&positions);
		circleRenderer.updateUniforms(
			scene->viewportData,
			WindowData{ SCREEN_WIDTH, SCREEN_HEIGHT }
		);
		circleRenderer.render(true);

		std::vector<InstanceData> rectPositions = scene->getRectData();
		std::vector<LineData> lines = scene->getLineData();
		for (LineData ld : lines)
			rectPositions.push_back(ld.convertToInstanceData());
		rectangleRenderer.setInstanceData(&rectPositions);
		rectangleRenderer.updateUniforms(
			scene->viewportData,
			WindowData{ SCREEN_WIDTH, SCREEN_HEIGHT }
		);
		rectangleRenderer.render(false);

		ImGui::Render();

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		SDL_GL_SwapWindow(window);
		pr.end();
	}
	delete scene;
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
		if (ImGui::GetIO().WantCaptureMouse)
			continue;

		if (e.type == SDL_QUIT)
			quit = true;
		if (e.type == SDL_MOUSEBUTTONDOWN)
		{
			int x, y;
			SDL_GetMouseState(&x, &y);
			float yOffset = (float)(SCREEN_HEIGHT / 2);
			float xOffset = (float)(SCREEN_WIDTH / 2);

			ViewportData& data = scene->viewportData;

			Vec2 pos = Vec2(x-xOffset, y-yOffset);
			pos.x /= (SCREEN_WIDTH / 2) / ((float)SCREEN_WIDTH/SCREEN_HEIGHT);
			pos.y /= -(SCREEN_HEIGHT / 2);
			pos *= data.zoom;
			Vec2 rot = data.rotation;
			Vec2 invRot = Vec2(rot.x / (rot.x * rot.x + rot.y * rot.y), -rot.y / (rot.x * rot.x + rot.y * rot.y));
			pos -= data.offset;
			pos = Vec2(-pos.x * invRot.x + pos.y * invRot.y, - pos.x * invRot.y - pos.y * invRot.x);
			pos *= -1;

			scene->mouseDownEvent(pos);
		}
	}
}