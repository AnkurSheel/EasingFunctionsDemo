//  *******************************************************************************************************************
//  Game   version:  1.0   Ankur Sheel  date: 2013/04/02
//  *******************************************************************************************************************
//  purpose:	
//  *******************************************************************************************************************
#include "stdafx.h"
#include "Game.h"
#include "EasingDemoView.h"
#include "BaseControl.hxx"
#include "EasingDemoEntityFactory.h"
#include "ComponentFactory.h"
#include "EntityManager.hxx"
#include "EventManager.hxx"
#include "EventFactory.h"
#include "Timer.hxx"
#include "HighScoreTable.hxx"
#include "UiManager.hxx"
#include "ProcessManager.hxx"
#include "RenderSystem.h"
#include "Config.h"
#include "KeyboardController.hxx"
#include "Ball.h"

using namespace Graphics;
using namespace Base;
using namespace GameBase;
using namespace Utilities;

//  *******************************************************************************************************************
cGame::cGame(const Base::cString & Name) 
	: cBaseApp(Name)
{
}

//  *******************************************************************************************************************
cGame::~cGame()
{
	VCleanup();
}

//  *******************************************************************************************************************
void cGame::VOnInitialization(const HINSTANCE & hInstance, const int CmdShow, const cString & OptionsFile)
{
	cBaseApp::VOnInitialization(hInstance, CmdShow, OptionsFile);

	if(m_Quitting)
	{
		return;
	}

	m_pConfig = DEBUG_NEW cConfig();
	m_pConfig->VInitialize("GameConfig");

	VGetProcessManager()->VAttachProcess(shared_ptr<Utilities::cProcess>(DEBUG_NEW cRenderSystem()));

	m_pEntityManager = (MakeStrongPtr<IEntityManager>(cServiceLocator::GetInstance()->GetService<IEntityManager>()));
	if (m_pEntityManager != NULL)
	{
		m_pBall = dynamic_cast<cBall *>(m_pEntityManager->VAddEntity("ball"));
	}

	shared_ptr<IUiManager> pUIManager = (MakeStrongPtr<IUiManager>(cServiceLocator::GetInstance()->GetService<IUiManager>()));

	m_pGameTimer = ITimer::CreateTimer();
	m_pGameTimer->VStartTimer();
}

//  *******************************************************************************************************************
void cGame::VCreateHumanView()
{
	m_pHumanView = DEBUG_NEW cEasingDemoView();
}

//  *******************************************************************************************************************
void cGame::VOnUpdate()
{
	if(m_Quitting)
	{
		return;
	}
	cBaseApp::VOnUpdate();

	if(m_pGameTimer != NULL)
	{
		if (m_pEntityManager != NULL)
		{
			m_pEntityManager->VUpdate(m_pGameTimer->VGetDeltaTime());
		}

		if(m_pHumanView!= NULL)
		{
			m_pHumanView->VOnUpdate(m_pGameTimer->VGetRunningTicks(), m_pGameTimer->VGetDeltaTime());
		}
	}
}

//  *******************************************************************************************************************
void cGame::VCleanup()
{
	VGetProcessManager()->VDetachProcess(cRenderSystem::m_Type);

	if (m_pHumanView->m_pAppWindowControl != NULL)
	{
		m_pHumanView->m_pAppWindowControl->VRemoveChildControl("HUD");
	}

	SafeDelete(&m_pGameTimer);

	if (m_pEntityManager != NULL)
	{
		m_pEntityManager->VDeleteAllEntities();
	}
	m_pEntityManager.reset();
	cBaseApp::VCleanup();
}

//  ********************************************************************************************************************
void cGame::VCreateEntityFactory()
{
	shared_ptr<IEntityManager> pEntityManager = (MakeStrongPtr<IEntityManager>(cServiceLocator::GetInstance()->GetService<IEntityManager>()));
	if (pEntityManager != NULL)
	{
		pEntityManager->VInitializeEntityFactory(shared_ptr<cEntityFactory>(DEBUG_NEW cEasingDemoEntityFactory()));
	}
}

//  *******************************************************************************************************************
void cGame::VInitializeGameOptions()
{

}

//  *******************************************************************************************************************
void cGame::OnKeyPressed(int key)
{
	if (IKeyboardController::Instance()->VIsKeyPressed(VK_ESCAPE))
	{
		m_Quitting = true;
		PostQuitMessage(0);
		return;
	}

	m_pBall->OnKeyPressed(key);
}
//  *******************************************************************************************************************
IBaseApp * IGame::CreateGame(const cString Name)
{
	return DEBUG_NEW cGame(Name);
}
