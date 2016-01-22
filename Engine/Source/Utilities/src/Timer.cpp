#include "stdafx.h"
#include "Timer.h"

using namespace Utilities;
using namespace std;

class cTimer::cTimerImpl
{
public:
	cTimerImpl();
	~cTimerImpl();
	void StartTimer();
	void PauseTimer();
	void StopTimer();
	void OnUpdate();
	float GetDeltaTime() const;
	void CalculateFPS();
	bool IsStopped() const { return m_TimerStopped; }
	float GetFPS() const { return m_FPS; }
	float GetRunningTime() const { return m_RunningTime; }
	TICK GetRunningTicks() const { return m_TotalsFrames; }

private:
	float m_RunningTime;  /// The total time this timer has been running
	float m_DeltaTime;  /// The time that has elapsed between 2 frames
	float m_FPS;  /// The FPS
	uint64 m_TicksPerSecond;  /// The number of times the system counter fires in 1 second
	uint64 m_CurrentTime;  /// The time at this instant
	uint64 m_LastUpdateTime;  /// The time at which the timer was last updated
	uint64 m_LastFPSUpdateTime;  /// The time at which the FPS was last calculated
	uint64 m_FPSUpdateInterval;  /// The time interval between calls to update the FPS
	uint64 m_NumFrames;  /// The numbers of frames that have been rendered since last FPS Calculation
	uint64 m_TotalsFrames;  /// The total number of frames been rendered since this timer has been running
	bool m_TimerStopped;  /// True if the timer is stopped
};

//  *******************************************************************************************************************
cTimer::cTimer()
	: m_pImpl(DEBUG_NEW cTimerImpl())
{
}

//  *******************************************************************************************************************
cTimer::~cTimer()
{
}

//  *******************************************************************************************************************
void cTimer::StartTimer()
{
	m_pImpl->StartTimer();
}

//  *******************************************************************************************************************
void cTimer::PauseTimer()
{
	m_pImpl->PauseTimer();
}

//  *******************************************************************************************************************
void cTimer::StopTimer()
{
	m_pImpl->StopTimer();
}

//  *******************************************************************************************************************
void cTimer::OnUpdate()
{
	m_pImpl->OnUpdate();
}

//  *******************************************************************************************************************
bool cTimer::IsStopped() const
{
	return m_pImpl->IsStopped();
}

//  *******************************************************************************************************************
float cTimer::GetFPS() const
{
	return m_pImpl->GetFPS();
}

//  *******************************************************************************************************************
float cTimer::GetRunningTime() const
{
	return m_pImpl->GetRunningTime();
}

//  *******************************************************************************************************************
TICK cTimer::GetRunningTicks() const
{
	return m_pImpl->GetRunningTicks();
}

//  *******************************************************************************************************************
float cTimer::GetDeltaTime() const
{
	return m_pImpl->GetDeltaTime();
}

//  *******************************************************************************************************************
unique_ptr<cTimer> cTimer::CreateTimer()
{
	return unique_ptr<cTimer>(DEBUG_NEW cTimer());
}

//  *******************************************************************************************************************
cTimer::cTimerImpl::cTimerImpl()
	: m_CurrentTime(0)
	, m_LastUpdateTime(0)
	, m_LastFPSUpdateTime(0)
	, m_NumFrames(0)
	, m_TotalsFrames(0)
	, m_FPS(0.0f)
	, m_RunningTime(0.0f)
	, m_DeltaTime(0.0f)
	, m_TimerStopped(true)
{
	// Check how many times per second the system counter fires
	if (!QueryPerformanceFrequency((LARGE_INTEGER *)&m_TicksPerSecond))
	{
		SP_ASSERT_FATAL(false).SetCustomMessage("No support for the high performance counter");
	}

	// update every half second
	m_FPSUpdateInterval = m_TicksPerSecond >> 1;
}

//  *******************************************************************************************************************
cTimer::cTimerImpl::~cTimerImpl()
{
}

//  *******************************************************************************************************************
void cTimer::cTimerImpl::StartTimer()
{
	if (m_TimerStopped)
	{
		// Get the current value of the high-resolution performance counter.
		QueryPerformanceCounter((LARGE_INTEGER *)&m_LastUpdateTime);
		m_TimerStopped = false;
	}
}

//  *******************************************************************************************************************
void cTimer::cTimerImpl::PauseTimer()
{
	if (!m_TimerStopped)
	{
		uint64 stopTime = 0;

		// Get the current value of the high-resolution performance counter.
		QueryPerformanceCounter((LARGE_INTEGER *)&stopTime);

		m_RunningTime += static_cast<float>(stopTime - m_LastUpdateTime) / m_TicksPerSecond;
		m_TimerStopped = true;
	}
}

//  *******************************************************************************************************************
void cTimer::cTimerImpl::OnUpdate()
{
	if (!m_TimerStopped)
	{
		// Get the current value of the high-resolution performance counter.
		QueryPerformanceCounter((LARGE_INTEGER *)&m_CurrentTime);

		m_DeltaTime = (m_CurrentTime - m_LastUpdateTime) / static_cast<float>(m_TicksPerSecond);
		m_RunningTime += m_DeltaTime;

		m_NumFrames++;
		m_TotalsFrames++;

		CalculateFPS();

		m_LastUpdateTime = m_CurrentTime;
	}
}

//  *******************************************************************************************************************
void cTimer::cTimerImpl::StopTimer()
{
	m_CurrentTime = 0;
	m_LastUpdateTime = 0;
	m_LastFPSUpdateTime = 0;
	m_NumFrames = 0;
	m_TotalsFrames = 0;
	m_FPS = 0.0f;
	m_RunningTime = 0.0f;
	m_DeltaTime = 0.0f;
	m_TimerStopped = true;
}

//  *******************************************************************************************************************
void cTimer::cTimerImpl::CalculateFPS()
{
	if (m_CurrentTime - m_LastFPSUpdateTime >= m_FPSUpdateInterval)
	{
		float currentTime = static_cast<float>(m_CurrentTime) / m_TicksPerSecond;
		float lastTime = static_cast<float>(m_LastFPSUpdateTime) / m_TicksPerSecond;
		m_FPS = m_NumFrames / (currentTime - lastTime);

		m_LastFPSUpdateTime = m_CurrentTime;
		m_NumFrames = 0;
	}
}

//  *******************************************************************************************************************
float cTimer::cTimerImpl::GetDeltaTime() const
{
	if (m_TimerStopped)
	{
		return 0.0f;
	}
	return m_DeltaTime;
}
