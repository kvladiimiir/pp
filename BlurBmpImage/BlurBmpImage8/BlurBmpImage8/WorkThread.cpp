#include "pch.h"
#include "WorkThread.h"

WorkThread::WorkThread(std::vector<ITask*>& tasks, int threadNum)
	: m_threadNum(threadNum)
{
	handles.resize(tasks.size());
	for (int i = 0; i < tasks.size(); i++)
	{
		handles[i] = CreateThread(NULL, 0, &ThreadProc, tasks[i], CREATE_SUSPENDED, NULL);
	}
}

void WorkThread::WorkWithThread()
{
	for (const auto& handle : handles)
	{
		ResumeThread(handle);
	}

	WaitForMultipleObjects((DWORD)handles.size(), handles.data(), true, INFINITE);
}

void WorkThread::WorkWithPool()
{
	int index = 0;
	for (size_t i = 0; i < handles.size(); i++)
	{
		index++;
		ResumeThread(handles[i]);
		if (index == m_threadNum)
		{
			index = 0;
			WaitForMultipleObjects((DWORD)i + 1, handles.data(), true, INFINITE);
		}
	}
	WaitForMultipleObjects((DWORD)handles.size(), handles.data(), true, INFINITE);
}