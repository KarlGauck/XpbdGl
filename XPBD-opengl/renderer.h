#pragma once

#include <string>
#include <vector>

#include "instanceData.h"
#include "viewportData.h"
#include "windowData.h"

class Renderer {
public:
	Renderer();
	Renderer(std::vector<float> &vertexData, std::vector<unsigned int> &indexData,
		std::string vertexShaderPath, std::string fragmentShaderPath);

	void render(bool clear);
	void setInstanceData(std::vector<InstanceData>* buffer);
	void updateUniforms(ViewportData viewportData, WindowData windowData);

	float ZOOM = 3.5;
	float VIEW_WIDTH = ZOOM * 16.f, VIEW_HEIGHT = ZOOM * 9.f;


private:
	static const int INSTANCE_BUFFER_SIZE = 10;
	int fanCount = 50;

	unsigned int instanceCount;
	std::vector<InstanceData>* instanceData;

	std::vector<float> vertexData;
	std::vector<unsigned int> indexData;

	unsigned int program;
	unsigned int circleVAO;
	unsigned int circleVBO;
	unsigned int circleEBO;
	unsigned int circleInstanceVBO;

	int viewportSizeUniformLoc;
	int viewportOffsetUniformLoc;
	int viewportRotationUniformLoc;

	std::string getShaderSource(std::string path);
	void shaderLog(int shaderId);
	void programLog(int programId);

	void setupShaders(std::string vertexShaderPath, std::string fragmentShaderPath);
	void setupVAOs();
	void setupUniformLocs();

	void pushVertexData();
	void pushInstanceData();
};
