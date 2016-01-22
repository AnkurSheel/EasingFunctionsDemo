#include "stdafx.h"
#include "SystemChecker.h"
#include <direct.h>
#include <math.h>
#include <time.h>
#include <mmsystem.h>
#include <intrin.h>

using namespace Base;
using namespace Utilities;

cSystemChecker * cSystemChecker::m_pResourceChecker = NULL;

//  *******************************************************************************************************************
cSystemChecker::cSystemChecker()
	: m_TotalPhysicalMemory(0)
	, m_AvailablePhysicalMemory(0)
	, m_TotalVirtualMemory(0)
	, m_AvailableVirtualMemory(0)
	, m_AvailableHardDiskSpace(0)
	, m_TotalHardDiskSpace(0)
	, m_CPUSpeed(0)
	, m_Initialized(false)
{
}

//  *******************************************************************************************************************
cSystemChecker::~cSystemChecker()
{
}

//  *******************************************************************************************************************
bool cSystemChecker::VIsOnlyInstance(const cString & gameTitle)
{
	// Find the window. If active, set and return false
	// Only one game instance may have this mutex at a time...

	// cppcheck-suppress leakReturnValNotUsed
	CreateMutex(NULL, TRUE, gameTitle.GetData());

	if (GetLastError() != ERROR_SUCCESS)
	{
		HWND hWnd = FindWindow(NULL, gameTitle.GetData());
		if (hWnd)
		{
			// An instance of your game is already running.
			ShowWindow(hWnd, SW_SHOWNORMAL);
			SetFocus(hWnd);
			SetForegroundWindow(hWnd);
			SetActiveWindow(hWnd);
			return false;
		}
	}
	return true;
}

//  *******************************************************************************************************************
void cSystemChecker::VCheckHardDiskSpace(const uint64 diskSpaceNeeded)
{
	SP_ASSERT_ERROR(m_AvailableHardDiskSpace > diskSpaceNeeded)(diskSpaceNeeded)(m_AvailableHardDiskSpace).SetCustomMessage("Not Enough HardDisk Space");
}

//  *******************************************************************************************************************
void cSystemChecker::VCheckMemory(const uint64 physicalRAMNeeded, const uint64 virtualRAMNeeded)
{
	SP_ASSERT_ERROR(m_AvailablePhysicalMemory > physicalRAMNeeded)(physicalRAMNeeded)(m_AvailablePhysicalMemory).SetCustomMessage("Not Enough Physical Memory");
	SP_ASSERT_ERROR(m_AvailableVirtualMemory > virtualRAMNeeded)(virtualRAMNeeded)(m_AvailableVirtualMemory).SetCustomMessage("Not Enough Virtual Memory");

	char * pBuff = DEBUG_NEW char[virtualRAMNeeded];
	SP_ASSERT_ERROR(pBuff != NULL).SetCustomMessage("System Lied: Not Enough Virtual Memory");
	if (pBuff)
	{
		SafeDeleteArray(&pBuff);
	}
}

//  *******************************************************************************************************************
void cSystemChecker::VCheckCPUSpeed(const uint32 minSpeedNeeded)
{
	SP_ASSERT_FATAL(m_CPUSpeed > minSpeedNeeded)(minSpeedNeeded)(m_CPUSpeed).SetCustomMessage("CPU is too slow");
}

//  *******************************************************************************************************************
bool cSystemChecker::Initialize()
{
	if (!m_Initialized)
	{
		m_Initialized = true;
		CalculateHardDiskSpace();
		CalculateRAM();
		CalculateCPUSpeed();
	}
	return m_Initialized;
}

//  *******************************************************************************************************************
void cSystemChecker::CalculateHardDiskSpace()
{
	const int drive = _getdrive();
	struct _diskfree_t diskfree = {};
	_getdiskfree(drive, &diskfree);

	m_TotalHardDiskSpace = static_cast<uint64>(diskfree.total_clusters) * diskfree.sectors_per_cluster * diskfree.bytes_per_sector;
	m_AvailableHardDiskSpace = static_cast<uint64>(diskfree.avail_clusters) * diskfree.sectors_per_cluster * diskfree.bytes_per_sector;
}

//  *******************************************************************************************************************
void cSystemChecker::CalculateRAM()
{
	MEMORYSTATUSEX status;
	status.dwLength = sizeof (status);
	GlobalMemoryStatusEx(&status);

	m_TotalPhysicalMemory = static_cast<uint64>(status.ullTotalPhys);
	m_AvailablePhysicalMemory= static_cast<uint64>(status.ullAvailPhys);
	m_AvailableVirtualMemory = static_cast<uint64>(status.ullAvailVirtual);
	m_TotalVirtualMemory= static_cast<uint64>(status.ullTotalVirtual);
}

//  *******************************************************************************************************************
void cSystemChecker::CalculateCPUSpeed()
{
	unsigned long bufSize = sizeof(unsigned long);
	unsigned long type = REG_DWORD;
	HKEY hKey;
	// open the key where the proc speed is hidden:
	long error = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 0, KEY_READ, &hKey);
	if (error == ERROR_SUCCESS)
	{
		UINT64 speed;
		// query the key:
		RegQueryValueEx(hKey, "~MHz", NULL, &type, (LPBYTE) &speed, &bufSize);
		m_CPUSpeed = static_cast<uint64>(speed);
	}
}

//  *******************************************************************************************************************
ISystemChecker * ISystemChecker::Instance()
{
	if (cSystemChecker::m_pResourceChecker == NULL)
	{
		cSystemChecker::m_pResourceChecker = DEBUG_NEW cSystemChecker();
		cSystemChecker::m_pResourceChecker->Initialize();
	}
	return cSystemChecker::m_pResourceChecker;
}

//  *******************************************************************************************************************
void ISystemChecker::Destroy()
{
	SafeDelete(&cSystemChecker::m_pResourceChecker);
}
