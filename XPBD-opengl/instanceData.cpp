#include "instanceData.h"

InstanceData LineData::convertToInstanceData()
{
	float len = start.distance(end);
	return InstanceData
	{
		end-(end-start)*.5f,
		Vec2(len, width),
		(end-start)/len,
		{color[0], color[1], color[2], color[3]}
	};
}