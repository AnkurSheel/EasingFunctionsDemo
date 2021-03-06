//  *******************************************************************************************************************
//  Score   version:  1.0   Ankur Sheel  date: 2013/03/14
//  *******************************************************************************************************************
//
//  *******************************************************************************************************************
#ifndef Score_h__
#define Score_h__

#include "GameBaseDefines.h"

namespace GameBase
{
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /// @brief Base class to hold scores. This class should be derived to hold
  /// game specific scores
  ///
  ///
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  class cScore
  {
  public:
    GAMEBASE_API cScore();
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Constructor
    ///
    /// @param[in] PlayerName The name of the player.
    /// @param[in] Score The score of the player.
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    GAMEBASE_API cScore(const Base::cString& PlayerName, const int Score);
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Destructor
    ///
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    GAMEBASE_API ~cScore();
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Returns the current score
    ///
    /// @return The current score
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    GAMEBASE_API int GetScore() const;
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Increments the score with /cDeltaValue. If /cDeltaValue is negative
    /// decrements the score
    ///
    /// @param[in] DeltaValue The name of the player
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    GAMEBASE_API void UpdateScore(const int DeltaValue);
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Sets the score to /cScore
    ///
    /// @param[in] Score The new value of the score
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    GAMEBASE_API void SetScore(const int Score);
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Returns the player name
    ///
    /// @return The player name
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    GAMEBASE_API Base::cString GetPlayerName() const;
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Sets the player name to /cPlayerName
    ///
    /// @param[in] PlayerName The name of the player.
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    GAMEBASE_API void SetPlayerName(const Base::cString& PlayerName);

  public:
    GAMEBASE_API bool operator>(cScore const& s) const;
    GAMEBASE_API bool operator<(cScore const& s) const;

  protected:
    Base::cString m_PlayerName;  ///< The name of the player
    int m_Score;                 ///< The score of the player
  };
}  // namespace GameBase
#endif  // Score_h__
