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
#include "Timer.h"
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
cGame::cGame(const Base::cString& Name)
  : cBaseApp(Name)
  , m_pBall(NULL)
{
}

//  *******************************************************************************************************************
cGame::~cGame()
{
  VCleanup();
}

//  *******************************************************************************************************************
void cGame::VOnInitialization(const HINSTANCE& hInstance, const int CmdShow, const cString& OptionsFile)
{
  cBaseApp::VOnInitialization(hInstance, CmdShow, OptionsFile);

  if (m_Quitting)
  {
    return;
  }

  m_pConfig = DEBUG_NEW cConfig();
  m_pConfig->VInitialize("GameConfig");

  VGetProcessManager()->VAttachProcess(shared_ptr<cProcess>(DEBUG_NEW cRenderSystem()));

  m_pEntityManager = (MakeStrongPtr<IEntityManager>(cServiceLocator::GetInstance()->GetService<IEntityManager>()));
  if (m_pEntityManager != NULL)
  {
    m_pBall = dynamic_pointer_cast<cBall>(m_pEntityManager->VAddEntity("ball"));
  }

  shared_ptr<IUiManager> pUIManager =
      (MakeStrongPtr<IUiManager>(cServiceLocator::GetInstance()->GetService<IUiManager>()));

  m_pHUDScreen = pUIManager->VCreateUI("HUD");
  m_pHumanView->m_pAppWindowControl->VAddChildControl(m_pHUDScreen, weak_ptr<IBaseControl>());
  m_pTransitionXLabel = m_pHUDScreen->VFindChildControl("TransitionXLabel");
  m_pEaseTypeXLabel = m_pHUDScreen->VFindChildControl("EaseTypeXLabel");
  m_pTransitionYLabel = m_pHUDScreen->VFindChildControl("TransitionYLabel");
  m_pEaseTypeYLabel = m_pHUDScreen->VFindChildControl("EaseTypeYLabel");

  UpdateTransitionText();
}

//  *******************************************************************************************************************
void cGame::VCreateHumanView()
{
  m_pHumanView = DEBUG_NEW cEasingDemoView();
}

//  *******************************************************************************************************************
void cGame::VOnUpdate()
{
  if (m_Quitting)
  {
    return;
  }
  cBaseApp::VOnUpdate();

  if (m_pGameTimer != NULL)
  {
    if (m_pEntityManager != NULL)
    {
      m_pEntityManager->VUpdate(m_pGameTimer->GetDeltaTime());
    }

    if (m_pHumanView != NULL)
    {
      m_pHumanView->VOnUpdate(m_pGameTimer->GetRunningTicks(), m_pGameTimer->GetDeltaTime());
    }
  }
  UpdateTransitionText();
}

//  *******************************************************************************************************************
void cGame::VCleanup()
{
  VGetProcessManager()->VDetachProcess(cRenderSystem::m_Type);

  if (m_pHumanView->m_pAppWindowControl != NULL)
  {
    m_pHumanView->m_pAppWindowControl->VRemoveChildControl("HUD");
  }

  if (m_pEntityManager != NULL)
  {
    m_pEntityManager->VDeleteAllEntities();
  }
  m_pEntityManager.reset();
  cBaseApp::VCleanup();
}

//  ********************************************************************************************************************
void cGame::VCreateAndSetEntityFactory()
{
  shared_ptr<IEntityManager> pEntityManager =
      (MakeStrongPtr<IEntityManager>(cServiceLocator::GetInstance()->GetService<IEntityManager>()));
  if (pEntityManager != NULL)
  {
    pEntityManager->VSetAndInitializeEntityFactory(std::unique_ptr<cEntityFactory>(DEBUG_NEW cEasingDemoEntityFactory()));
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

  UpdateTransitionText();
}

//  *******************************************************************************************************************
void cGame::UpdateTransitionText()
{
  MakeStrongPtr(m_pTransitionXLabel)->VSetText("X transition: " + m_pBall->GetCurrentXTransition());
  MakeStrongPtr(m_pEaseTypeXLabel)->VSetText("X Ease Type: " + m_pBall->GetCurrentXEasingType());
  MakeStrongPtr(m_pTransitionYLabel)->VSetText("Y transition: " + m_pBall->GetCurrentYTransition());
  MakeStrongPtr(m_pEaseTypeYLabel)->VSetText("Y Ease Type: " + m_pBall->GetCurrentYEasingType());
}

//  *******************************************************************************************************************
IBaseApp* IGame::CreateGame(const cString& Name)
{
  return DEBUG_NEW cGame(Name);
}
