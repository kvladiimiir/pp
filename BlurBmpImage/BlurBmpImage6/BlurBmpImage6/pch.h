// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.

#ifndef PCH_H
#define PCH_H

#include <iostream>
#include "LogBuffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <windows.h>
#include <stdint.h>
#include "bitmap_image.hpp"
#include <fstream>
#include <ctime>
// bitmap file header

using namespace std;

enum ThreadPriority
{
	below_normal = -1,
	normal,
	above_normal
};

struct ThreadData
{
	bitmap_image ibmp;
	bitmap_image* obmp;
	int startIndex;
	int endIndex;
	int numberOfThread;
	clock_t start;
	LogBuffer* logBuffer;
};

struct InputData
{
	string inputImgName;
	string outputImgName;
	int threads;
	int cores;
	vector<ThreadPriority> prioritiesThreads;
};

const int border = 5;

#endif //PCH_H