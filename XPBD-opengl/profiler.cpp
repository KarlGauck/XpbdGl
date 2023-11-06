#include "profiler.h"
#include <chrono>
#include <numeric>

Profiler::Profiler(std::string name, int averageSize) : name{ name }, averageSize{ averageSize } {};

void Profiler::start() 
{
	this->currentTime = std::chrono::high_resolution_clock::now();
}

void Profiler::end() 
{
	auto end = std::chrono::high_resolution_clock::now();
	long long deltatime = std::chrono::duration_cast<std::chrono::nanoseconds>(end - this->currentTime).count();
	avgNums.insert(avgNums.begin(), deltatime);
	if (avgNums.size() > averageSize)
		avgNums.pop_back();
	long long sum = std::accumulate(avgNums.begin(), avgNums.end(), 0LL);
	duration = (sum / avgNums.size());
}
