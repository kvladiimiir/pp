// NewThreadNumbers.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <windows.h>
#include <time.h>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;
CRITICAL_SECTION CriticalSection;

DWORD WINAPI ThreadProc(CONST LPVOID workingVariable)
{
	EnterCriticalSection(&CriticalSection);
	int j = *(int*)workingVariable;
	for (int i = 0; i < 100000; i++)
	{
		j = j + 1;
	}
	*(int*)workingVariable = j;
	LeaveCriticalSection(&CriticalSection);
	ExitThread(0);
}

int main(int argc, char* argv[])
{
	InitializeCriticalSection(&CriticalSection);
	int threadsNumber = 2;
	HANDLE* handles = new HANDLE[threadsNumber];
	int WorkingVariable = 0;
	unsigned affinityMask1 = 0x00000000;
	unsigned affinityMask2 = 0x0000000C;
	for (int i = 0; i < threadsNumber; i++)
	{
		SetThreadAffinityMask(handles[i], affinityMask2);
		handles[i] = CreateThread(NULL, 0, &ThreadProc, &WorkingVariable, CREATE_SUSPENDED, NULL);
		ResumeThread(handles[i]);
	}

	WaitForMultipleObjects(threadsNumber, handles, true, INFINITE);
	DeleteCriticalSection(&CriticalSection);
	cout << WorkingVariable;
	return 0;
}
