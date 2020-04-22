// NewThreadNumbers.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include "IWorker.h"
#include "ITask.h"


using namespace std;

class Task : ITask
{
public:
	void Execute() override;
};

DWORD WINAPI ThreadProc(LPVOID lpParam)
{
	ITask *task = (ITask*)lpParam;
	task->Execute();
	ExitThread(0);
}

void Task::Execute()
{
	std::cout << "Task new";
}

class Worker : public IWorker
{
public:
	bool ExecuteTask(Task & task)
	{
		if (IsBusy())
		{
			return false;
		}
		Task * taskPtr = new Task();
		int threadsNumber = 5;

		handle = CreateThread(NULL, 0, &ThreadProc, &task, CREATE_SUSPENDED, NULL);
		std::cout << '\n';
		ResumeThread(handle);

		WaitForSingleObject(handle, INFINITE);

		isComplete = true;
		return isComplete;
	}

	bool IsBusy()
	{
		LPDWORD status;

		if (GetExitCodeThread(handle, status))
		{
			return true;
		}

		return isComplete;
	}

private:
	HANDLE handle = new HANDLE;
	bool isComplete = false;
};

int main(int argc, char* argv[])
{
	return 0;
}
