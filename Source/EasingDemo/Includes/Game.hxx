//  *******************************************************************************************************************
//  Game   version:  1.0   Ankur Sheel  date: 2013/04/02
//  *******************************************************************************************************************
//  purpose:	
//  *******************************************************************************************************************
#ifndef Game_hxx__
#define Game_hxx__

#include "GameDefines.h"
#include "Vector3.h"

namespace GameBase
{
	class IBaseApp;
}

class IGame 
{
public:
	virtual ~IGame(){}
	BREAKOUT_API static GameBase::IBaseApp * CreateGame(const Base::cString Name);
};
#endif // Game_hxx__