#pragma once
#include "ITask.h"
#include <vector>

class WorkThread
{
public:
	WorkThread(std::vector<ITask*>& tasks, int threadNum);
	void WorkWithThread();
	void WorkWithPool();
private:
	std::vector<HANDLE> handles;
	int m_threadNum;
};

static DWORD WINAPI ThreadProc(CONST LPVOID lpParam)
{
	ITask* task = static_cast<ITask*>(lpParam);
	task->BlurImage();
	ExitThread(0);
}