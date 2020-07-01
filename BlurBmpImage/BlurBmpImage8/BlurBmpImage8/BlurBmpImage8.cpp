#include "pch.h"

InputData* ReadData(char* argv[])
{
	string threadsString = argv[5];
	string blocksString = argv[2];
	string outputDir = argv[4];
	string inputDir = argv[3];
	int blocks = stoi(blocksString);
	int threads = stoi(threadsString);
	InputData* result = new InputData();
	result->blocksNum = blocks;
	result->inputDirName = inputDir;
	result->outputDirName = outputDir;
	result->threads = threads;
	string modeArg = argv[1];

	if (modeArg == "pool")
	{
		result->mode = PoolMode;
	}
	else
	{
		result->mode = DefaultThreadMode;
	}

	return result;
}

float GetPeriod(clock_t start)
{
	clock_t end = clock();
	return (float)difftime(end, start);
}

vector<string> WorkWithDirectory(string dirName)
{
	vector<string> result;
	for (auto& p : fs::directory_iterator(dirName))
	{
		std::cout << p.path() << '\n';
		result.push_back(p.path().string());
	}

	return result;
}

void BlurImage(InputData* inputData)
{
	int threads = inputData->threads;
	int blocks = inputData->blocksNum;

	if (!fs::exists(inputData->inputDirName))
	{
		cout << "Error find input dir\n";
		throw exception();
	}

	if (!fs::exists(inputData->outputDirName))
	{
		fs::create_directory(inputData->outputDirName);
	}

	vector<string> imagesToBlur;

	imagesToBlur = WorkWithDirectory(inputData->inputDirName);

	for (size_t j = 0; j < imagesToBlur.size(); j++)
	{
		bitmap_image inputImg(imagesToBlur[j]);
		bitmap_image outputImg(inputImg);

		ThreadData* threadsData = new ThreadData[threads];

		int colWidth = div(inputImg.width(), blocks).quot;

		vector<ITask*> tasks;

		for (int i = 0; i < blocks; i++)
		{
			int first = (i == 0) ? 0 : (i * blocks) + 1;
			int second = (i == blocks - 1) ? inputImg.width() - 1 : (i + 1) * blocks;

			ThreadData threadData = { inputImg, &outputImg, first, second };
			threadsData[i] = threadData;

			tasks.push_back(new Blur(threadData));
		}

		WorkThread work(tasks, blocks);
		if (inputData->mode == PoolMode)
		{
			work.WorkWithPool();
		}
		else
		{
			work.WorkWithThread();
		}

		outputImg.save_image(inputData->outputDirName + "/blur" + to_string(j) + ".bmp");
	}
}

int main(int argc, char* argv[])
{
	if (argc == 2 && strcmp(argv[1], "/?") == 0)
	{
		cout << "Input: BlurBmpImage8.exe mode(pool/thread) blocksNum inputImgsDirectoryPath outputImgsDirectoryPath threads";
		return -1;
	}

	if (argc < 6)
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

		try
		{
			clock_t start = clock();

			BlurImage(inputData);

			clock_t period = clock() - start;
			cout << ((float)period / CLOCKS_PER_SEC) << '\n';
		}
		catch (exception)
		{
			cout << "Error in blur";
			return 1;
		}
	}

	return 0;
}