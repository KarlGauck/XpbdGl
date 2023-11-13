#include "verletScene.h"

#include "verletSolver.h"

VerletScene::VerletScene()
{
	solver = new VerletSolver();
	viewportData = ViewportData
	{
		3.5f,
		Vec2(0.0f, 0.0f)
	};
}

void VerletScene::mouseDownEvent(Vec2 pos)
{
	solver->addParticle(pos, Vec2(0.0, 0.0f));
}

std::vector<InstanceData> VerletScene::getCircleData()
{
	return solver->getCircleData();
}

std::vector<InstanceData> VerletScene::getRectData()
{
	return {};
}

std::vector<LineData> VerletScene::getLineData()
{
	return {};
}
