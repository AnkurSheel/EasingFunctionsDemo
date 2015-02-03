//  *******************************************************************************************************************
//  Ball   version:  1.0   Ankur Sheel  date: 2013/05/13
//  *******************************************************************************************************************
//  purpose:	
//  *******************************************************************************************************************
#include "stdafx.h"
#include "Ball.h"
#include "vector3.h"
#include "gameoptions.h"
#include "transformcomponent.h"
#include "physicscomponent.h"
#include "entitymanager.hxx"
#include "EventManager.hxx"
#include "TweenManager.h"
#include "KeyboardController.hxx"

using namespace Base;
using namespace GameBase;
using namespace Utilities;

const Base::cHashedString	cBall::m_EntityType = cHashedString("ball");

//  *******************************************************************************************************************
cBall::cBall()
	: m_ElapsedTime(0.0f)
	, m_CurrentTransition(EasingTransitions::Linear)
	, m_CurrenEasingType(EasingType::EaseIn)
{
}

//  *******************************************************************************************************************
cBall::~cBall()
{
	VCleanup();
}

//  *******************************************************************************************************************
void cBall::VInitialize()
{
	cBaseEntity::VInitialize();
	if(m_pTransFormComponent != NULL)
	{
		m_pTransFormComponent->SetPosition(cVector3(100.0f, 100.0f, 0.0f));
	}
	VOnInitialized();
}

//  *******************************************************************************************************************
void cBall::VCleanup()
{
	cBaseEntity::VCleanup();
}

//  *******************************************************************************************************************
cBall * cBall::CastToBall()
{
	return this;
}

//  *******************************************************************************************************************
void cBall::VOnUpdate(const float deltaTime)
{
	if(m_pTransFormComponent != NULL)
	{
		m_ElapsedTime += deltaTime;
		cVector3 startPosition = cVector3(100.0f, 100.0f, 0.0f);
		cVector3 endPosition = cVector3(500.0f, 500.0f, 0.0f);
		float duration = 5.0f;
		//cVector3 position = cVector3::Lerp(startPosition, endPosition, m_ElapsedTime/3.0f);
		shared_ptr<cTweenManager> pTweenManger = (MakeStrongPtr<cTweenManager>(cServiceLocator::GetInstance()->GetService<cTweenManager>()));
		if (pTweenManger != NULL)
		{
			cVector3 position = pTweenManger->Interpolate<cVector3>(m_CurrentTransition, m_CurrenEasingType, startPosition, endPosition, m_ElapsedTime, duration);
			m_pTransFormComponent->SetPosition(position);
			if (m_ElapsedTime > duration)
			{
				VOnRestart();
			}
		}
	}
}

//  *******************************************************************************************************************
void cBall::VOnRestart()
{
	m_ElapsedTime = 0.0f;
	if(m_pTransFormComponent != NULL)
	{
		m_pTransFormComponent->SetPosition(cVector3(100.0f, 100.0f, 0.0f));
	}
	cBaseEntity::VOnRestart();
}

//  *******************************************************************************************************************
cString cBall::GetCurrentTransition()
{
	return EasingTransitions::ToString(m_CurrentTransition);
}

//  *******************************************************************************************************************
cString cBall::GetCurrentEasingType()
{
	return EasingType::ToString(m_CurrenEasingType);
}

//  *******************************************************************************************************************
void cBall::OnKeyPressed(int key)
{
	if (IKeyboardController::Instance()->VIsKeyPressed('q') || IKeyboardController::Instance()->VIsKeyPressed('Q'))
	{
		m_CurrentTransition = (EasingTransitions::Enum)((int)(m_CurrentTransition) + 1);
		if (m_CurrentTransition == EasingTransitions::Total)
		{
			m_CurrentTransition = EasingTransitions::Linear;
		}
		VOnRestart();
	}

	if (IKeyboardController::Instance()->VIsKeyPressed('a') || IKeyboardController::Instance()->VIsKeyPressed('A'))
	{
		m_CurrenEasingType = (EasingType::Enum)((int)(m_CurrenEasingType) + 1);
		if (m_CurrenEasingType == EasingType::Total)
		{
			m_CurrenEasingType = EasingType::EaseIn;
		}
		VOnRestart();
	}
}
