#pragma once
#include "bitmap_image.hpp"
#include "ITask.h"
#include <vector>
#include "ThreadData.h"

class Blur : public ITask
{
public:
	Blur(ThreadData& threadData);
	void BlurImage() override;
private:
	std::vector<rgb_t> GetColors(int i, int j, const int width, const int height, bitmap_image& currImg, int& numOfPix);
	ThreadData m_threadData;
};