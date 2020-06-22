#include "pch.h"
#include "LogFileWriter.h"

LogFileWriter::LogFileWriter(std::string fileName)
	: m_output(std::ofstream(fileName))
{
}

void LogFileWriter::Write(std::string str)
{
	m_output << str;
}