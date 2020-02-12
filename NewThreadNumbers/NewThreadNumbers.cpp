// NewThreadNumbers.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <windows.h>
#include <string>
#include <iostream>

DWORD WINAPI ThreadProc(CONST LPVOID lpParam)
{
	std::cout << *(int*)lpParam << " number thread\n";
	ExitThread(0);
}

int main(int argc, char* argv[])
{
	int threadsNumber = std::stoi(argv[1]);
	HANDLE* handles = new HANDLE[threadsNumber];
	int* num = new int[threadsNumber];
	for (int i = 0; i < threadsNumber; i++)
	{
		num[i] = i + 1;
		handles[i] = CreateThread(NULL, 0, &ThreadProc, (LPVOID)&num[i], CREATE_SUSPENDED, NULL);
		ResumeThread(handles[i]);
	}

	WaitForMultipleObjects(threadsNumber, handles, true, INFINITE);
	return 0;
}
