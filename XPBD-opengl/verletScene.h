#pragma once

#include "scene.h"

class VerletScene: public Scene
{
public:
	VerletScene();

	void mouseDownEvent(Vec2 pos) override;

	std::vector<InstanceData> getCircleData() override;
	std::vector<InstanceData> getRectData() override;
	std::vector<LineData> getLineData() override;
};