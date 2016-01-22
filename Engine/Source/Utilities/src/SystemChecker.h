//  *******************************************************************************************************************
//  SystemChecker   version:  1.0   Ankur Sheel  date: 2013/04/24
//  *******************************************************************************************************************
//  purpose:apdapted from Game Coding Complete
//  *******************************************************************************************************************
#ifndef SystemChecker_h__
#define SystemChecker_h__

#include "SystemChecker.hxx"

namespace Utilities
{
	class cSystemChecker
		: public ISystemChecker
		, public Base::cNonCopyable
	{
	public:
		~cSystemChecker();

	private:
		cSystemChecker();
		bool VIsOnlyInstance(const Base::cString & gameTitle) OVERRIDE;
		void VCheckHardDiskSpace(const uint64 diskSpaceNeeded) OVERRIDE;
		void VCheckMemory(const uint64 physicalRAMNeeded, const uint64 virtualRAMNeeded) OVERRIDE;
		void VCheckCPUSpeed(const uint32 minSpeedNeeded) OVERRIDE;
		uint64 VGetTotalPhysicalMemory() const OVERRIDE { return m_TotalPhysicalMemory; }
		uint64 VGetAvailablePhysicalMemory() const OVERRIDE { return m_AvailablePhysicalMemory; }
		uint64 VGetTotalVirtualMemory() const OVERRIDE { return m_TotalVirtualMemory; }
		uint64 VGetAvailableVirtualMemory() const OVERRIDE { return m_AvailableVirtualMemory; }
		uint64 VGetTotalHardDiskSpace() const OVERRIDE { return m_TotalHardDiskSpace; }
		uint64 VGetAvailableHardDiskSpace() const OVERRIDE { return m_AvailableHardDiskSpace; }
		uint32 VGetCPUSpeed() const OVERRIDE { return m_CPUSpeed; }
		////////////////////////////////////////////////////////////////////////
		/// Initializes the System checker and calculates all the values
		///
		/// @return True if initialization is successful. False otherwise
		///
		////////////////////////////////////////////////////////////////////////
		bool Initialize();
		////////////////////////////////////////////////////////////////////////
		/// Calculates the total and available hard disk space
		///
		///
		///
		////////////////////////////////////////////////////////////////////////
		void CalculateHardDiskSpace();
		////////////////////////////////////////////////////////////////////////
		/// Calculates the total and available System RAM and VRAM
		///
		///
		///
		////////////////////////////////////////////////////////////////////////
		void CalculateRAM();
		////////////////////////////////////////////////////////////////////////
		/// Calculates the CPU Speed
		///
		///
		///
		////////////////////////////////////////////////////////////////////////
		void CalculateCPUSpeed();

	private:
		uint64 m_TotalPhysicalMemory;  /// The total system RAM (in MB)
		uint64 m_AvailablePhysicalMemory;  /// The available system RAM (in MB)
		uint64 m_TotalVirtualMemory;  /// The total VRAM (in MB)
		uint64 m_AvailableVirtualMemory;  /// The available VRAM (in MB)
		uint64 m_TotalHardDiskSpace;  /// The total hard disk space (in MB)
		uint64 m_AvailableHardDiskSpace;  /// The available hard disk space (in MB)
		uint32 m_CPUSpeed;  /// The CPU speed (in Mhz)
		bool m_Initialized;  /// True if initialized. False otherwise
		static cSystemChecker * m_pResourceChecker;

	private:
		friend static ISystemChecker * ISystemChecker::Instance();
		friend static void ISystemChecker::Destroy();
	};
}  // namespace Utilities
#endif  // SystemChecker_h__
