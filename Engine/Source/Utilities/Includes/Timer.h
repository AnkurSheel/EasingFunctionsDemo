//  *******************************************************************************************************************
//  Timer   version:  1.0   Ankur Sheel  date: 2011/02/15
//  *******************************************************************************************************************
//
//  *******************************************************************************************************************
#ifndef TIMER_H__
#define TIMER_H__

#include "UtilitiesDefines.h"

namespace Utilities
{
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /// Interface for the Timer Class
  ///
  ///
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  class cTimer : public Base::cNonCopyable
  {
  public:
    ~cTimer();
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Starts the timer
    ///
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    UTILITIES_API void StartTimer();
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Pauses the timer
    ///
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    UTILITIES_API void PauseTimer();
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Stops the timer and resets all the values
    ///
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    UTILITIES_API void StopTimer();
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Updates the timer. Calculates the time elapsed since the last update call. Updates the FPS and total
    /// running time
    ///
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    UTILITIES_API void OnUpdate();
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Check whether the timer is stopped or running
    ///
    /// @return True if the timer has stopped.
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    UTILITIES_API bool IsStopped() const;
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Get the FPS
    ///
    /// @return The FPS.
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    UTILITIES_API float GetFPS() const;
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Get the running time
    ///
    /// @return The running time.
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    UTILITIES_API float GetRunningTime() const;
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Get the number of frames that have elapsed
    ///
    /// @return The no. of frames that have elapsed
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    UTILITIES_API TICK GetRunningTicks() const;
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Gets the elapsed time between frames
    ///
    /// @return The elapsed time.
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    UTILITIES_API float GetDeltaTime() const;
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Creates and returns a pointer to an object of this class
    ///
    /// @return Returns a pointer to the timer
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    UTILITIES_API static std::unique_ptr<cTimer> CreateTimer();

  private:
    cTimer();

  private:
    class cTimerImpl;
    std::unique_ptr<cTimerImpl> m_pImpl;
  };
}  // namespace Utilities
#endif  // TIMER_H__
