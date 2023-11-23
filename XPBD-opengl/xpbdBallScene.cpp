#include "xpbdBallScene.h"
#include "xpbdSolver.h"
#include "verletSolver.h"

#include "distanceconstraint.h"
#include "densityconstraint.h"

#include "imgui.h"

#include <string>
#include <iostream>

XpbdBallScene::XpbdBallScene()
{
	solver = new XpbdSolver();;
	viewportData = ViewportData
	{
		70.f,
		Vec2(0.0f, 0.0f),
		Vec2(1.0f, 0.0f)
	};
}

static float zoom = 0.0f;

static float rotation = 0.0f;

static bool paused = false;
static bool showLinks = false;

void XpbdBallScene::step()
{
	XpbdSolver* solver = (XpbdSolver*)(this->solver);

	ImGui::Begin("Scenesettings");
	if (ImGui::Button("Pause"))
		paused = !paused;
	ImGui::Checkbox("Gravity: ", &(solver->gravity));
	ImGui::Checkbox("Show Links: ", &showLinks);
	ImGui::SliderFloat("Rotation", &rotation, -2*3.1415926535, 2 * 3.1415926535);
	ImGui::SliderFloat("Restdensity", &(DensityConstraint::RestDensity), 0.f, 150.f);
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
	for (int x = 0; x > -10; x--)
		for (int y = 0; y > -10; y--)
			solver->addParticle(pos + Vec2(x, y), Vec2(0.0, 0.0f));
}

std::vector<InstanceData> XpbdBallScene::getCircleData()
{
	return solver->getCircleData();
}

std::vector<InstanceData> XpbdBallScene::getRectData()
{
	float width = 200.f;
	float height = 100.0f;

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
	if (!showLinks)
		return {};
	std::vector<LineData> lines = std::vector<LineData>();

	XpbdSolver* xs = (XpbdSolver*)solver;
	for (DensityConstraint& constraint : DensityConstraint::constraints)
	{
		for (int neighbourIndex : constraint.neighbours)
		{
			Vec2 p1 = xs->particles[constraint.particle].pos;
			Vec2 p2 = xs->particles[neighbourIndex].pos;

			lines.push_back(LineData{
				p1,
				p2,
				0.3,
				{1.0f, 1.0f, 1.0f, 1.0f}
			});
		}
	}
	return lines;
}