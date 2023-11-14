#pragma once

#include "scene.h"

class XpbdBallScene: public Scene
{
public:
	XpbdBallScene();

	void step() override;
	void mouseDownEvent(Vec2 pos) override;

	std::vector<InstanceData> getCircleData() override;
	std::vector<InstanceData> getRectData() override;
	std::vector<LineData> getLineData() override;
};