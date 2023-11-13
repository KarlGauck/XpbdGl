#pragma once

#include "Vec2.h"
#include "Vec2.h"

struct InstanceData
{
	Vec2 position;
	Vec2 scale;
	Vec2 rotation;
	float color[4];
};

struct LineData
{
	Vec2 start;
	Vec2 end;
	float width;
	float color[4];

	InstanceData convertToInstanceData();
};
