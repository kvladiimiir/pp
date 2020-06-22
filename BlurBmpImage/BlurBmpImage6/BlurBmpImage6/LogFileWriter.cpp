#include "pch.h"
#include "LogFileWriter.h"

LogFileWriter::LogFileWriter(string fileName)
	: m_output(ofstream(fileName))
{
}

void LogFileWriter::Write(string str)
{
	m_output << str;
}