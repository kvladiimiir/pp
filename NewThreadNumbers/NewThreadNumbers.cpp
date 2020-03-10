// NewThreadNumbers.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <windows.h>
#include <time.h>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;
clock_t start;
ofstream fileOutput1("t1.log");
ofstream fileOutput2("t2.log");

DWORD WINAPI ThreadProc(CONST LPVOID lpParam)
{

	for (int i = 0; i < 125; i++)
	{
		clock_t stop = clock();
		if (*(int*)lpParam == 1)
		{
			fileOutput1 << ((float)stop - start) / CLOCKS_PER_SEC << '\n';
		}
		else
		{
			fileOutput2 << ((float)stop - start) / CLOCKS_PER_SEC << '\n';
		}
	}

	ExitThread(0);
}

int main(int argc, char* argv[])
{
	int threadsNumber = 2;
	HANDLE* handles = new HANDLE[threadsNumber];
	int* num = new int[threadsNumber];
	for (int i = 0; i < threadsNumber; i++)
	{
		start = clock();
		num[i] = i + 1;
		handles[i] = CreateThread(NULL, 0, &ThreadProc, (LPVOID)&num[i], CREATE_SUSPENDED, NULL);
		ResumeThread(handles[i]);
	}

	WaitForMultipleObjects(threadsNumber, handles, true, INFINITE);
	return 0;
}
