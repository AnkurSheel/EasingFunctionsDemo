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

#define AUTOMATIC

using namespace Base;
using namespace GameBase;
using namespace Utilities;

const cHashedString cBall::m_EntityType = cHashedString("ball");

//  *******************************************************************************************************************
cBall::cBall()
  : m_XTweenParams(0.5f, 3.0f, EasingTransitions::Linear, EasingType::EaseIn)
  , m_YTweenParams(0.5f, 3.0f, EasingTransitions::Linear, EasingType::EaseIn)
  , m_InitialPosition(200.0f, 200.0f, 0.0f)
  , m_FinalPosition(400.0f, 400.0f, 0.0f)
  , m_InitialScale(20.0f, 20.0f, 8.0f)
  , m_FinalScale(60.0f, 40.0f, 8.0f)
  , m_pTransformComponentStrongPtr(nullptr)
{
  m_XTweenParams.m_extraParams.overshootBackTween = 3.0f;
  m_YTweenParams.m_extraParams.overshootBackTween = 3.0f;

  // m_XTweenParams.m_RepeatCount = 2;
  // m_YTweenParams.m_RepeatCount = 2;
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

  m_pTransformComponentStrongPtr = MakeStrongPtr(m_pTransFormComponent);

  if (m_pTransformComponentStrongPtr != nullptr)
  {
    m_pTransformComponentStrongPtr->SetPosition(m_InitialPosition);
    m_pTransformComponentStrongPtr->SetSize(m_InitialScale);
  }

  m_Position = m_InitialPosition;
  m_Scale = m_InitialScale;

  m_XTweenParams.AddProperty(&(m_Position.x), m_FinalPosition.y);
  m_XTweenParams.AddProperty(&(m_Scale.x), m_FinalScale.x);

  m_YTweenParams.AddProperty(&(m_Position.y), m_FinalPosition.y);
  m_YTweenParams.AddProperty(&(m_Scale.y), m_FinalScale.y);

  cTweenParameters::TweenCallBackFn callbackTweenCompleted = bind(&cBall::OnTweenCompleted, this, _1);
  m_XTweenParams.RegisterCallBack(callbackTweenCompleted);

  ReInit();

  VOnInitialized(false);
}

//  *******************************************************************************************************************
void cBall::VCleanup()
{
  cBaseEntity::VCleanup();
}

//  *******************************************************************************************************************
cBall* cBall::CastToBall()
{
  return this;
}

//  *******************************************************************************************************************
void cBall::VOnUpdate(const float deltaTime)
{
  if (m_pTransformComponentStrongPtr != nullptr)
  {
    m_pTransformComponentStrongPtr->SetPosition(m_Position);
    m_pTransformComponentStrongPtr->SetSize(m_Scale);
  }
}

//  *******************************************************************************************************************
void cBall::VOnRestart()
{
#ifdef AUTOMATIC
  bool result = IncrementYEasingType();
  IncrementXEasingType();
  if (result)
  {
    IncrementYTransition();
    IncrementXTransition();
  }
#endif
  ReInit();

  cBaseEntity::VOnRestart();
}

//  *******************************************************************************************************************
void cBall::OnKeyPressed(int key)
{
#ifndef AUTOMATIC
  if (IKeyboardController::Instance()->VIsKeyPressed('q') || IKeyboardController::Instance()->VIsKeyPressed('Q'))
  {
    RemoveExistingTweens();
    IncrementXTransition();
    VOnRestart();
  }
  else if (IKeyboardController::Instance()->VIsKeyPressed('w') || IKeyboardController::Instance()->VIsKeyPressed('W'))
  {
    RemoveExistingTweens();
    IncrementXEasingType();
    VOnRestart();
  }
  else if (IKeyboardController::Instance()->VIsKeyPressed('a') || IKeyboardController::Instance()->VIsKeyPressed('A'))
  {
    RemoveExistingTweens();
    IncrementYTransition();
    VOnRestart();
  }
  else if (IKeyboardController::Instance()->VIsKeyPressed('s') || IKeyboardController::Instance()->VIsKeyPressed('S'))
  {
    RemoveExistingTweens();
    IncrementYEasingType();
    VOnRestart();
  }
  else if (IKeyboardController::Instance()->VIsKeyPressed('z') || IKeyboardController::Instance()->VIsKeyPressed('Z'))
  {
    RemoveExistingTweens();
    IncrementXTransition();
    IncrementYTransition();
    VOnRestart();
  }
  else if (IKeyboardController::Instance()->VIsKeyPressed('x') || IKeyboardController::Instance()->VIsKeyPressed('X'))
  {
    RemoveExistingTweens();
    IncrementXEasingType();
    IncrementYEasingType();
    VOnRestart();
  }
  else if (IKeyboardController::Instance()->VIsKeyPressed('r') || IKeyboardController::Instance()->VIsKeyPressed('R'))
  {
    RemoveExistingTweens();
    m_XTweenParams.m_TransitionType = EasingTransitions::Linear;
    m_XTweenParams.m_EasingType = EasingType::EaseIn;
    m_YTweenParams.m_TransitionType = EasingTransitions::Linear;
    m_YTweenParams.m_EasingType = EasingType::EaseIn;

    VOnRestart();
  }
#endif
}
//
//  *******************************************************************************************************************
cString cBall::GetCurrentXTransition()
{
  return EasingTransitions::ToString(m_XTweenParams.m_TransitionType);
}

//  *******************************************************************************************************************
cString cBall::GetCurrentXEasingType()
{
  return EasingType::ToString(m_XTweenParams.m_EasingType);
}

//  *******************************************************************************************************************
cString cBall::GetCurrentYTransition()
{
  return EasingTransitions::ToString(m_YTweenParams.m_TransitionType);
}

//  *******************************************************************************************************************
cString cBall::GetCurrentYEasingType()
{
  return EasingType::ToString(m_YTweenParams.m_EasingType);
}

//  *******************************************************************************************************************
bool cBall::IncrementXTransition()
{
  m_XTweenParams.m_TransitionType = (EasingTransitions::Enum)(static_cast<int>(m_XTweenParams.m_TransitionType) + 1);
  if (m_XTweenParams.m_TransitionType == EasingTransitions::Total)
  {
    m_XTweenParams.m_TransitionType = EasingTransitions::Linear;
    return true;
  }
  return false;
}

//  *******************************************************************************************************************
bool cBall::IncrementXEasingType()
{
  m_XTweenParams.m_EasingType = (EasingType::Enum)(static_cast<int>(m_XTweenParams.m_EasingType) + 1);
  if (m_XTweenParams.m_EasingType == EasingType::Total)
  {
    m_XTweenParams.m_EasingType = EasingType::EaseIn;
    return true;
  }
  return false;
}

//  *******************************************************************************************************************
bool cBall::IncrementYTransition()
{
  m_YTweenParams.m_TransitionType = (EasingTransitions::Enum)(static_cast<int>(m_YTweenParams.m_TransitionType) + 1);
  if (m_YTweenParams.m_TransitionType == EasingTransitions::Total)
  {
    m_YTweenParams.m_TransitionType = EasingTransitions::Linear;
    return true;
  }
  return false;
}

//  *******************************************************************************************************************
bool cBall::IncrementYEasingType()
{
  m_YTweenParams.m_EasingType = (EasingType::Enum)(static_cast<int>(m_YTweenParams.m_EasingType) + 1);
  if (m_YTweenParams.m_EasingType == EasingType::Total)
  {
    m_YTweenParams.m_EasingType = EasingType::EaseIn;
    return true;
  }
  return false;
}

//  *******************************************************************************************************************
void cBall::OnTweenCompleted(const cTweenParameters& params)
{
#ifdef AUTOMATIC
  VOnRestart();
#endif
}

//  *******************************************************************************************************************
void cBall::ReInit()
{
  if (m_pTransformComponentStrongPtr != nullptr)
  {
    m_Position = m_InitialPosition;
    m_Scale = m_InitialScale;

    m_pTransformComponentStrongPtr->SetPosition(m_InitialPosition);
    m_pTransformComponentStrongPtr->SetSize(m_InitialScale);
  }

  shared_ptr<cTweenManager> pTweenManger =
      (MakeStrongPtr<cTweenManager>(cServiceLocator::GetInstance()->GetService<cTweenManager>()));
  if (pTweenManger != NULL)
  {
    pTweenManger->AddTween(m_XTweenParams);
    pTweenManger->AddTween(m_YTweenParams);
  }
}

//  *******************************************************************************************************************
void cBall::RemoveExistingTweens()
{
  shared_ptr<cTweenManager> pTweenManger =
      (MakeStrongPtr<cTweenManager>(cServiceLocator::GetInstance()->GetService<cTweenManager>()));
  if (pTweenManger != NULL)
  {
    pTweenManger->RemoveTween(m_XTweenParams);
    pTweenManger->RemoveTween(m_YTweenParams);
  }
}
