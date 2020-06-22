#include "pch.h"
#include "LogBuffer.h"

LogBuffer::LogBuffer(LogFileWriter* logFileWriter)
	: m_criticalSection(CRITICAL_SECTION())
	, m_logFileWriter(logFileWriter)
{
	if (!InitializeCriticalSectionAndSpinCount(&m_criticalSection, 0x00000400))
	{
		throw exception("Critical section not initialize");
	}

	m_eventHandle = CreateEvent(nullptr, true, false, L"Event");

	if (!m_eventHandle)
	{
		throw exception("Event not initialize");
	}

	StartThread();
}

LogBuffer::~LogBuffer()
{
	CloseHandle(m_eventHandle);
	CloseHandle(m_threadHandle);

	if (&m_criticalSection)
	{
		DeleteCriticalSection(&m_criticalSection);
	}
}

void LogBuffer::StartThread()
{
	m_threadHandle = CreateThread(nullptr, 0, &LogSizeMonitoringThread, (void*)this, 0, nullptr);
}

void LogBuffer::Push(string value)
{
	linkedList.Push(value);
}

void LogBuffer::CheckSizeAndPushData(string value)
{
	if (&m_criticalSection)
	{
		EnterCriticalSection(&m_criticalSection);
	}

	if (linkedList.GetListSize() >= MAX_SIZE)
	{
		SetEvent(m_eventHandle);

		if (WaitForSingleObject(m_threadHandle, INFINITE) == WAIT_OBJECT_0)
		{
			ResetEvent(m_eventHandle);

			StartThread();
		}
	}

	Push(value);

	if (&m_criticalSection)
	{
		LeaveCriticalSection(&m_criticalSection);
	}
}

DWORD WINAPI LogBuffer::LogSizeMonitoringThread(const LPVOID lpParam)
{
	LogBuffer* data = (LogBuffer*)lpParam;

	if (WaitForSingleObject(data->m_eventHandle, INFINITE) == WAIT_OBJECT_0)
	{
		auto listSize = data->linkedList.GetListSize();

		for (size_t i = 0; i < listSize; i++)
		{
			data->m_logFileWriter->Write(data->linkedList.GetListHead());

			data->linkedList.Pop();
		}
	}

	ExitThread(0);
}