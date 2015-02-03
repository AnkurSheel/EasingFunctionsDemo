//  *******************************************************************************************************************
//  Game   version:  1.0   Ankur Sheel  date: 2013/04/02
//  *******************************************************************************************************************
//  purpose:	
//  *******************************************************************************************************************
#ifndef Game_h__
#define Game_h__

#include "Game.hxx"
#include "BaseApp.h"

namespace Graphics
{
	class IBaseControl;
}

namespace GameBase
{
	class cHumanView;
	class cGameElementDef;
	class cComponentFactory;
	class IHighScoreTable;
	class IBaseEntity;
	class IEntityManager;
}

namespace AI
{
	struct Telegram;
}

class cGameFlowStateMachine;
class cEasingDemoEntityFactory;
class cBall;

class cGame 
	: public IGame
	, public GameBase::cBaseApp
{
public:
	cGame(const Base::cString & Name);

private:
	~cGame();
	void VOnInitialization(const HINSTANCE & hInstance, const int CmdShow, const Base::cString & OptionsFile) OVERRIDE;
	void VCreateEntityFactory() OVERRIDE;
	void VInitializeGameOptions() OVERRIDE;
	void VOnUpdate() OVERRIDE;
	void VCleanup() OVERRIDE;
	void VCreateHumanView() OVERRIDE;
	void OnKeyPressed(int key);

private:
	shared_ptr<GameBase::IEntityManager> m_pEntityManager;

	cBall* m_pBall;

private:
	friend class cEasingDemoView;
};
#endif // Game_h__