#include "stdafx.h"
#include "EasingDemoView.h"
#include "Camera.hxx"
#include "MessageDispatchManager.hxx"
#include "Game.h"
#include "Vector3.h"
#include "Messages.h"
#include "EntityManager.hxx"
#include "ProcessManager.hxx"
#include "RenderSystem.h"
#include "KeyboardController.hxx"
#include "EventManager.hxx"
#include "ControlStructures.h"
#include "BaseControl.hxx"
#include "Ball.h"

using namespace Utilities;
using namespace Graphics;
using namespace GameBase;
using namespace Base;

//  *******************************************************************************************************************
cEasingDemoView::cEasingDemoView()
	: m_FirstUpdate(true)
{

}

//  *******************************************************************************************************************
cEasingDemoView::~cEasingDemoView()
{
}

void cEasingDemoView::VOnCreateDevice(IBaseApp* pBaseApp)
{
	cHumanView::VOnCreateDevice(pBaseApp);

	cLabelControlDef timerLabelDef;
	timerLabelDef.strControlName = "TimerLabel";
	timerLabelDef.strFont= "arial"; 
	timerLabelDef.textColor = cColor::GREEN;
	timerLabelDef.fTextHeight = 30;
	timerLabelDef.vPosition = cVector2::Zero;
	timerLabelDef.bAutoSize = false;
	timerLabelDef.vSize = cVector2(150, 30);
	m_pTimerLabel = shared_ptr<IBaseControl>(IBaseControl::CreateLabelControl(timerLabelDef));
	m_pAppWindowControl->VAddChildControl(m_pTimerLabel);

	if (m_pCamera)
	{
		m_pCamera->VSetPosition(cVector3(0.0f, 0.0f, -20.0f));
		m_pCamera->VUpdate();
	}
}

//  *******************************************************************************************************************
void cEasingDemoView::VOnUpdate(const TICK tickCurrent, const float fElapsedTime)
{
	cHumanView::VOnUpdate(tickCurrent, fElapsedTime);
	if (m_FirstUpdate)
	{
		m_FirstUpdate = false;
		cGame * pGame = dynamic_cast<cGame *>(m_pGame);
		m_pTimerLabel->VSetText(pGame->m_pBall->GetCurrentTransition() + "\n" + pGame->m_pBall->GetCurrentEasingType());
	}
}

//  *******************************************************************************************************************
bool cEasingDemoView::VOnMsgProc( const Base::AppMsg & msg )
{
	if(!cHumanView::VOnMsgProc(msg))
	{
		if(msg.m_uMsg == WM_KEYDOWN)
		{
			if(!IKeyboardController::Instance()->VIsKeyLocked(msg.m_wParam))
			{
				IKeyboardController::Instance()->VOnKeyDown(msg.m_wParam);

				if (msg.m_wParam == VK_ESCAPE
					|| msg.m_wParam == 'q' || msg.m_wParam == 'Q'
					|| msg.m_wParam == 'a' || msg.m_wParam == 'A')
				{
					//// lock the ESC key
					IKeyboardController::Instance()->VLockKey(msg.m_wParam);
					cGame * pGame = dynamic_cast<cGame *>(m_pGame);
					pGame->OnKeyPressed(msg.m_wParam);
				}
				if (msg.m_wParam == 'q' || msg.m_wParam == 'Q'
					|| msg.m_wParam == 'a' || msg.m_wParam == 'A')
				{
					cGame * pGame = dynamic_cast<cGame *>(m_pGame);
					m_pTimerLabel->VSetText(pGame->m_pBall->GetCurrentTransition() + "\n" + pGame->m_pBall->GetCurrentEasingType());
				}
			}
		}
		else if (msg.m_uMsg == WM_KEYUP)
		{
			IKeyboardController::Instance()->VUnlockKey(msg.m_wParam);
			IKeyboardController::Instance()->VOnKeyUp(msg.m_wParam);
		}
	}
	return true;
}

//  *********************************************************************************************************************************
void cEasingDemoView::VRenderPrivate()
{
	ProcessList pProcessList;
	m_pGame->VGetProcessManager()->VGetProcesses(cRenderSystem::m_Type, pProcessList);
	for (auto curProcess = pProcessList.begin(); curProcess != pProcessList.end(); curProcess++)
	{
		shared_ptr<cRenderSystem> p = dynamic_pointer_cast<cRenderSystem>(*curProcess);
		p->Render(m_pCamera);
	}
	cHumanView::VRenderPrivate();
}
