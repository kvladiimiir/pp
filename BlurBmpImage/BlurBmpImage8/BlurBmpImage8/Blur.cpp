#include "pch.h"
#include "Blur.h"

Blur::Blur(ThreadData& threadData)
	: m_threadData(threadData)
{
}

std::vector<rgb_t> Blur::GetColors(int i, int j, const int width, const int height, bitmap_image& currImg, int& numOfPix)
{
	vector<rgb_t> resultColorsVector;

	for (int k = -border; k <= border; k++)
	{
		for (int l = -border; l <= border; l++)
		{
			if ((i + k) < width && (i + k) >= 0)
			{
				if (j + l < height && j + l >= 0)
				{
					rgb_t pixel = currImg.get_pixel(i + k, j + l);

					resultColorsVector.push_back(pixel);

					numOfPix++;
				}
			}
		}
	}

	return resultColorsVector;
}

void Blur::BlurImage()
{
	bitmap_image iimage = m_threadData.ibmp;
	bitmap_image* oimage = m_threadData.obmp;

	for (int i = m_threadData.startIndex; i <= m_threadData.endIndex; i++)
	{
		for (unsigned int j = 0; j < oimage->height(); j++)
		{
			int currR = 0;
			int currG = 0;
			int currB = 0;
			int pixNum = 0;

			vector<rgb_t> pixels = GetColors(i, j, oimage->width(), oimage->height(), iimage, pixNum);

			for (auto& item : pixels)
			{
				currR += item.red;
				currG += item.green;
				currB += item.blue;
			}

			oimage->set_pixel(i, j, currR / pixNum,
				currG / pixNum,
				currB / pixNum);
		}
	}
}