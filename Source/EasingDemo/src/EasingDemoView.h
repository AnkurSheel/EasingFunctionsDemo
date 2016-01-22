//  *******************************************************************************************************************
//  BreakoutView   version:  1.0   Ankur Sheel  date: 2013/04/02
//  *******************************************************************************************************************
//  purpose:	
//  *******************************************************************************************************************
#ifndef BreakoutView_h__
#define BreakoutView_h__

#include "HumanView.h"

class cGame;

namespace GameBase
{
	class IBaseApp;
}

class cEasingDemoView 
	: public GameBase::cHumanView
{
public:
	cEasingDemoView();
	~cEasingDemoView();
	void VOnCreateDevice(GameBase::IBaseApp * pBaseApp) OVERRIDE;
	void VOnUpdate(const TICK tickCurrent, const float fElapsedTime) OVERRIDE;
	void VRenderPrivate() OVERRIDE;
	bool VOnMsgProc(const Base::AppMsg & msg ) OVERRIDE;
};
#endif // BreakoutView_h__
