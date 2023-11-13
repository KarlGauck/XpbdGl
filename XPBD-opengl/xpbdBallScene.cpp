#include "xpbdBallScene.h"
#include "xpbdSolver.h"
#include "verletSolver.h"

XpbdBallScene::XpbdBallScene()
{
	solver = new XpbdSolver();;
	viewportData = ViewportData
	{
		3.5f,
		Vec2(0.0f, 0.0f)
	};
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