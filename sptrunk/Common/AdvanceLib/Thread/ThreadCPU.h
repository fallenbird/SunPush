#include "stdafx.h"

int GetProcessCount(void)
{
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);

	return sysInfo.dwNumberOfProcessors;
}

static CCriticalSection s_critical;
DWORD GetCPUUsage(HANDLE hThread)
{
	s_critical.Lock();

	// Declare four FILETIME.
	FILETIME ftCreate, ftExit, ftKernel, ftUser;
	// Declare the following variable to convert FILE to 100-nanosecond unit.
	LARGE_INTEGER lpIntKernelStart, lpIntUserStart, lpIntKernelEnd, lpIntUserEnd;

	// Record the start time.
	clock_t startTime = clock();

	// Get the first file time.
	if (!GetThreadTimes(hThread, &ftCreate, &ftExit, &ftKernel, &ftUser))
	{
		return 0;
	}
	// Convert FILETIME to 100-nanosecond.
	lpIntKernelStart.QuadPart = MAKELONGLONG(ftKernel.dwLowDateTime, ftKernel.dwHighDateTime);
	lpIntUserStart.QuadPart = MAKELONGLONG(ftUser.dwLowDateTime, ftUser.dwHighDateTime);

	Sleep(10);    // Some action, like this.

	// Get the second file time.
	if (!GetThreadTimes(hThread, &ftCreate, &ftExit, &ftKernel, &ftUser))
	{
		return 0;
	}
	// Convert FILETIME to 100-nanosecond.
	lpIntKernelEnd.QuadPart = MAKELONGLONG(ftKernel.dwLowDateTime, ftKernel.dwHighDateTime);
	lpIntUserEnd.QuadPart = MAKELONGLONG(ftUser.dwLowDateTime, ftUser.dwHighDateTime);

	// Calculate the runtime that kernel time add user time.
	LONGLONG lRunTime = (lpIntKernelEnd.QuadPart - lpIntKernelStart.QuadPart) + (lpIntUserEnd.QuadPart - lpIntUserStart.QuadPart) / 10000;
	// Calculate the system time lapsed.
	LONGLONG lLapse = (LONGLONG)(clock() - startTime);

	// Get count of CPU.
	int CPUCount = GetProcessCount();

	// Percent = (kerneltime + usertime) / (now - starttime) / numberofcpus.
	int nPercent = (int)(lRunTime * 100 / lLapse / CPUCount);

	s_critical.Unlock();

	return nPercent;
}
