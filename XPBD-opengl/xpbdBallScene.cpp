#include "xpbdBallScene.h"
#include "xpbdSolver.h"
#include "verletSolver.h"

#include "imgui.h"

#include <string>

XpbdBallScene::XpbdBallScene()
{
	solver = new XpbdSolver();;
	viewportData = ViewportData
	{
		35.0f,
		Vec2(0.0f, 0.0f),
		Vec2(1.0f, 0.0f)
	};
}

static float zoom = 0.0f;

static float rotation = 0.0f;

static bool paused = false;

void XpbdBallScene::step()
{
	XpbdSolver* solver = (XpbdSolver*)(this->solver);

	ImGui::Begin("Scenesettings");
	if (ImGui::Button("Pause"))
		paused = !paused;
	ImGui::Checkbox("Gravity: ", &(solver->gravity));
	ImGui::SliderFloat("Rotation", &rotation, -2*3.1415926535, 2 * 3.1415926535);
	ImGui::SliderFloat2("Offset", (float*) & (viewportData.offset), -50.f, 50.f);
	ImGui::End();
	viewportData.rotation = Vec2((float)cos(rotation), sin(rotation));

	ImGui::Begin("Particles");

	for (int particleIndex = 0; particleIndex < solver->particles.size();  particleIndex++)
	{
		ImGui::PushID(particleIndex);
		std::string name = "Particle " + std::to_string(particleIndex);
		if (ImGui::CollapsingHeader(name.c_str()))
		{
			ImGui::SliderFloat("test", &(solver->particles[particleIndex].radius), 0.5f, 2.f);
		}
		ImGui::PopID();
	}
	ImGui::End();
}

void XpbdBallScene::mouseDownEvent(Vec2 pos)
{
	solver->addParticle(pos, Vec2(0.0, 0.0f));
}

std::vector<InstanceData> XpbdBallScene::getCircleData()
{
	return solver->getCircleData();
}

std::vector<InstanceData> XpbdBallScene::getRectData()
{
	float width = 100.f;
	float height = 50.0f;

	return {
		InstanceData {
			Vec2(0.0f, -(height / 2) - .5f),
			Vec2(width, 1.0f),
			Vec2(1.0f, 0.0f),
			{1.0f, 1.0f, 1.0f, 1.0f}
		},
		InstanceData {
			Vec2(0.0f, (height / 2) + .5f),
			Vec2(width, 1.0f),
			Vec2(1.0f, 0.0f),
			{1.0f, 1.0f, 1.0f, 0.0f}
		},
		InstanceData {
			Vec2( - (width / 2) - .5f, 0.0f),
			Vec2(1.0f, height),
			Vec2(1.0f, 0.0f),
			{1.0f, 1.0f, 1.0f, 0.0f}
		},
		InstanceData {
			Vec2((width/2) + .5f, 0.0f),
			Vec2(1.0f, height),
			Vec2(1.0f, 0.0f),
			{1.0f, 1.0f, 1.0f, 1.0f}
		}
	};
}

std::vector<LineData> XpbdBallScene::getLineData()
{
	return {};
}