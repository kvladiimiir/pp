#include "pch.h"

InputData* ReadData(char* argv[])
{
	string threadsString = argv[3];
	string coresString = argv[4];

	InputData* result = new InputData();
	result->inputImgName = argv[1];
	result->outputImgName = argv[2];
	result->cores = stoi(coresString);
	result->threads = stoi(threadsString);
	return result;
}

vector<rgb_t> GetColors(int i, int j, int width, int height, bitmap_image& currImg, int& numOfPix)
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

DWORD WINAPI ThreadProc(CONST LPVOID lpParam)
{
	const ThreadData* data = static_cast<ThreadData*>(lpParam);

	bitmap_image iimage = data->ibmp;
	bitmap_image* oimage = data->obmp;

	for (int i = data->startIndex; i <= data->endIndex; i++)
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

	ExitThread(0);
}

void BlurImage(InputData* inputData)
{
	int threads = inputData->threads;
	int cores = inputData->cores;
	bitmap_image inputImg(inputData->inputImgName);
	bitmap_image outputImg(inputData->inputImgName);

	ThreadData* threadsData = new ThreadData[threads];

	int colNumber = div(inputImg.width(), threads).quot;

	for (int i = 0; i < threads; i++)
	{
		int first = (i == 0) ? 0 : (i * colNumber) + 1;
		int second = (i == threads - 1) ? inputImg.width() - 1 : (i + 1) * colNumber;

		ThreadData threadData = { inputImg, &outputImg, first, second };
		threadsData[i] = threadData;
	}

	const int affinityMask = (1 << cores) - 1;

	HANDLE* handles = new HANDLE[threads];
	for (int i = 0; i < threads; i++)
	{
		handles[i] = CreateThread(NULL, 0, &ThreadProc, &threadsData[i], CREATE_SUSPENDED, NULL);
		SetThreadAffinityMask(handles[i], affinityMask);
	}

	for (int i = 0; i < threads; i++)
	{
		ResumeThread(handles[i]);
	}

	WaitForMultipleObjects(threads, handles, true, INFINITE);

	outputImg.save_image(inputData->outputImgName);
}

int main(int argc, char* argv[])
{
	if (argc != 5)
	{
		cout << "Error arg number\n";
	}
	else
	{
		InputData* inputData = new InputData();
		try
		{
			inputData = ReadData(argv);
		}
		catch (invalid_argument)
		{
			cout << "Error threads number ore cores number.";
			return 1;
		}

		clock_t start = clock();
		BlurImage(inputData);
		clock_t period = clock() - start;
		cout << period << '\n';
	}

	return 0;
}