#pragma once

#include <string>
#include <vector>

class Renderer {
public:
	Renderer();
	Renderer(std::vector<float> &vertexData, std::vector<unsigned int> &indexData,
		std::string vertexShaderPath, std::string fragmentShaderPath);

	void render(bool clear);
	void setInstanceData(std::vector<float>* buffer);

	float VIEW_WIDTH = 1.2f * 16.f, VIEW_HEIGHT = 1.2f * 9.f;

private:
	int fanCount = 50;

	unsigned int instanceCount;
	std::vector<float>* instanceData;

	std::vector<float> vertexData;
	std::vector<unsigned int> indexData;


	unsigned int program;
	unsigned int circleVAO;
	unsigned int circleVBO;
	unsigned int circleEBO;
	unsigned int circleInstanceVBO;

	int viewWidthUniformLoc;
	int viewHeightUniformLoc;

	std::string getShaderSource(std::string path);
	void shaderLog(int shaderId);
	void programLog(int programId);

	void setupShaders(std::string vertexShaderPath, std::string fragmentShaderPath);
	void setupVAOs();
	void setupUniformLocs();

	void updateUniforms();
	void pushVertexData();
	void pushInstanceData();
};
