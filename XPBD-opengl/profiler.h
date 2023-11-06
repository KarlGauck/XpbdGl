#pragma once

#include <iostream>
#include <vector>
#include <chrono>
#include <string>

class Profiler {
public:
	Profiler(std::string name, int averageSize);
	void start();
	void end();
	int duration;
private:
	std::string name;
	std::chrono::high_resolution_clock::time_point currentTime;
	long long averageSize;
	std::vector<int> avgNums;
};