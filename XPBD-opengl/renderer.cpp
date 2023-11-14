#define GLEW_STATIC

#include <glew.h>
#include <SDL_opengl.h>
#include <gl/GLU.h>

#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>
#include <string>
#include <iostream>
#include <fstream>

#include "instanceData.h"
#include "renderer.h"


Renderer::Renderer() 
{}

Renderer::Renderer(std::vector<float> &vertexData, std::vector<unsigned int> &indexData,
	std::string vertexShaderPath, std::string fragmentShaderPath)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	this->vertexData = vertexData;
	this->indexData = indexData;

	setupShaders(vertexShaderPath, fragmentShaderPath);
	setupVAOs();
	setupUniformLocs();
	pushVertexData();
}

void Renderer::render(bool clear) 
{
	if (clear)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	pushInstanceData();

	glBindVertexArray(circleVAO);
	glDrawElementsInstanced(GL_TRIANGLES, indexData.size(), GL_UNSIGNED_INT, 0, instanceCount);
	glBindVertexArray(0);
}

void Renderer::setInstanceData(std::vector<InstanceData> *buffer) 
{
	instanceCount = buffer->size();// / INSTANCE_BUFFER_SIZE;
	instanceData = buffer;
}

std::string Renderer::getShaderSource(std::string path) 
{
	std::string result = "";
	std::string line;
	std::ifstream filestream(path);
	if (!filestream.is_open()) {
		std::cout << "Unable to read shaderSource" << std::endl;
	}

	int lineIndex = 0;
	while (getline(filestream, line)) {
		if (lineIndex != 0)
			result += "\n";
		result += line;
		lineIndex++;
	}

	filestream.close();
	std::cout << result << std::endl;
	return (result + "\0");
}

void Renderer::shaderLog(int shaderId) 
{
	int success;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
	if (success)
		return;
	char infoLog[512];
	glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
	std::cout << "Could not load shader: " << infoLog << std::endl;
}

void Renderer::programLog(int program) 
{
	int success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (success)
		return;
	char infoLog[512];
	glGetProgramInfoLog(program, 512, NULL, infoLog);
	std::cout << "Could not link program: " << infoLog << std::endl;
}


void Renderer::setupShaders(std::string vertexShaderPath, std::string fragmentShaderPath) 
{
	unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
	std::string vertexSourceString = getShaderSource(vertexShaderPath);
	const char* vertexSource = vertexSourceString.c_str();
	glShaderSource(vs, 1, &vertexSource, NULL);
	glCompileShader(vs);
	shaderLog(vs);

	unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
	std::string fragmentSourceString = getShaderSource(fragmentShaderPath);
	const char* fragmentSource = fragmentSourceString.c_str();
	glShaderSource(fs, 1, &fragmentSource, NULL);
	glCompileShader(fs);
	shaderLog(fs);

	program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	programLog(program);

	glDeleteShader(vs);
	glDeleteShader(fs);
}

void Renderer::setupVAOs() 
{
	glGenBuffers(1, &circleVBO);
	glGenBuffers(1, &circleEBO);
	glGenBuffers(1, &circleInstanceVBO);
	glGenVertexArrays(1, &circleVAO);

	glBindVertexArray(circleVAO);
	glBindBuffer(GL_ARRAY_BUFFER, circleVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, circleEBO);

	int circleVertexStride = 2 * sizeof(float);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, circleVertexStride, 0);
	glEnableVertexAttribArray(0);

	int circleInstanceVertexStride = INSTANCE_BUFFER_SIZE * sizeof(float);
	glBindBuffer(GL_ARRAY_BUFFER, circleInstanceVBO);

	// Offset
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, circleInstanceVertexStride, (void*)0); 
	glVertexAttribDivisor(1, 1); 
	glEnableVertexAttribArray(1); 
	
	// Scale
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, circleInstanceVertexStride, (void*)(2*sizeof(float)));
	glVertexAttribDivisor(2, 1);
	glEnableVertexAttribArray(2);

	// Rotatiton (Complex Number)
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, circleInstanceVertexStride, (void*)(4*sizeof(float)));
	glVertexAttribDivisor(3, 1);
	glEnableVertexAttribArray(3);

	// Color
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, circleInstanceVertexStride, (void*)(6 * sizeof(float)));
	glVertexAttribDivisor(4, 1);
	glEnableVertexAttribArray(4);

	glUseProgram(program);

	glBindVertexArray(0);
}

void Renderer::setupUniformLocs() 
{
	viewportSizeUniformLoc = glGetUniformLocation(program, "viewportSize");
	viewportOffsetUniformLoc = glGetUniformLocation(program, "viewportOffset");
	viewportRotationUniformLoc = glGetUniformLocation(program, "viewportRotation");
}

void Renderer::updateUniforms(ViewportData viewportData, WindowData windowData) 
{
	float whRatio = (float)windowData.width/windowData.height;

	glUniform2f(viewportSizeUniformLoc, whRatio * viewportData.zoom, viewportData.zoom);
	glUniform2f(viewportOffsetUniformLoc, viewportData.offset.x, viewportData.offset.y);
	glUniform2f(viewportRotationUniformLoc, viewportData.rotation.x, viewportData.rotation.y);
}

void Renderer::pushVertexData() 
{
	glBindVertexArray(circleVAO);

	glBindBuffer(GL_ARRAY_BUFFER, circleVBO);
	glBufferData(GL_ARRAY_BUFFER, (2 * (fanCount+1))*sizeof(float), vertexData.data(), GL_STATIC_DRAW);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, fanCount * sizeof(unsigned int) * 3, indexData.data(), GL_STATIC_DRAW);
	glBindVertexArray(0);
} 

void Renderer::pushInstanceData() 
{
	glBindVertexArray(circleVAO);

	glBindBuffer(GL_ARRAY_BUFFER, circleInstanceVBO);
	glBufferData(GL_ARRAY_BUFFER, instanceCount * INSTANCE_BUFFER_SIZE * sizeof(float), instanceData->data(), GL_DYNAMIC_DRAW);

	glBindVertexArray(0);
}
