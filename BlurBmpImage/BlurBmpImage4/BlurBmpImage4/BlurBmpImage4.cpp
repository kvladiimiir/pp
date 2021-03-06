﻿#include "pch.h"

InputData* ReadData(char* argv[])
{
	string threadsString = argv[3];
	string coresString = argv[4];
	int threads = stoi(threadsString);

	vector<ThreadPriority> prioritiesCurrThreads;
	InputData* result = new InputData();
	result->inputImgName = argv[1];
	result->outputImgName = argv[2];
	result->cores = stoi(coresString);
	result->threads = threads;

	for (int i = 5; i < 5 + threads; i++)
	{
		if (strcmp(argv[i], "below_normal") == 0)
		{
			prioritiesCurrThreads.push_back(ThreadPriority::below_normal);
			cout << "bellow\n";
		}
		else if (strcmp(argv[i], "normal") == 0) {
			prioritiesCurrThreads.push_back(ThreadPriority::normal);
			cout << "normal\n";
		}
		else if (strcmp(argv[i], "above_normal") == 0) {
			prioritiesCurrThreads.push_back(ThreadPriority::above_normal);
			cout << "above\n";
		}
	}
	result->prioritiesThreads = prioritiesCurrThreads;
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
	ofstream* oFiles = data->outputFiles;

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

			clock_t stop = clock();
			*oFiles << data->numberOfThread << "\t" << (((float)stop - data->start) / CLOCKS_PER_SEC) * 1000 << '\n';
		}
	}

	ExitThread(0);
}

void BlurImage(InputData* inputData, clock_t startTime)
{
	int threads = inputData->threads;
	int cores = inputData->cores;
	bitmap_image inputImg(inputData->inputImgName);
	bitmap_image outputImg(inputData->inputImgName);
	ofstream* outputFilesStreams = new ofstream[threads];

	ThreadData* threadsData = new ThreadData[threads];
	ThreadPriority* threadsPriorities = new ThreadPriority[threads];

	int colNumber = div(inputImg.width(), threads).quot;

	for (int i = 0; i < threads; i++)
	{
		outputFilesStreams[i] = ofstream("t" + to_string(i) + ".txt");
		threadsPriorities[i] = inputData->prioritiesThreads[i];

		int first = (i == 0) ? 0 : (i * colNumber) + 1;
		int second = (i == threads - 1) ? inputImg.width() - 1 : (i + 1) * colNumber;

		ThreadData threadData = { inputImg, &outputImg, first, second, i + 1, startTime, &outputFilesStreams[i] };
		threadsData[i] = threadData;
	}

	const int affinityMask = (1 << cores) - 1;

	HANDLE* handles = new HANDLE[threads];
	for (int i = 0; i < threads; i++)
	{
		handles[i] = CreateThread(NULL, 0, &ThreadProc, &threadsData[i], CREATE_SUSPENDED, NULL);
		SetThreadAffinityMask(handles[i], affinityMask);
		SetThreadPriority(handles[i], static_cast<int>(inputData->prioritiesThreads[i]));
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
	if (argc == 2 && strcmp(argv[1], "/?") == 0)
	{
		cout << "Input: BlurBmpImage4.exe inputFile.bmp outputFile.bmp threads cores priorities(separated by space)\n Exmple priorities:\n below_normal \n normal \n above_normal";
		return -1;
	}

	if (argc < 5)
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
		BlurImage(inputData, start);
		clock_t period = clock() - start;
		cout << period << '\n';
	}

	return 0;
}