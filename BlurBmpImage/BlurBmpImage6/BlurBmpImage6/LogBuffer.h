#pragma once

#include "LinkedList.h"
#include "LogFileWriter.h"
#include <windows.h>
#include <fstream>

const int MAX_SIZE = 500;

class LogBuffer {
public:
	LogBuffer(LogFileWriter* logFileWriter);
	void CheckSizeAndPushData(std::string value);
	void Push(std::string value);

	~LogBuffer();

private:
	size_t m_size = 0;
	CRITICAL_SECTION m_criticalSection;
	HANDLE m_eventHandle;
	HANDLE m_threadHandle;
	std::string m_fileName;
	LinkedList linkedList;
	std::ofstream m_outputFile;
	LogFileWriter* m_logFileWriter;
	void StartThread();
	static DWORD WINAPI LogSizeMonitoringThread(CONST LPVOID lp_param);
};