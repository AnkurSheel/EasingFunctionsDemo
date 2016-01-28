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

const cHashedString cBall::m_EntityType = cHashedString("ball");

//  *******************************************************************************************************************
cBall::cBall()
  : m_ElapsedTime(0.0f)
  , m_XMovementParams(EasingTransitions::Linear, EasingType::EaseIn, 200.0f, 400.0f, 3.0f)
  , m_YMovementParams(EasingTransitions::Linear, EasingType::EaseIn, 200.0f, 400.0f, 3.0f)
  , m_pTransformComponentStrongPtr(nullptr)
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

  m_pTransformComponentStrongPtr = MakeStrongPtr(m_pTransFormComponent);
  if (m_pTransformComponentStrongPtr != nullptr)
  {
    m_pTransformComponentStrongPtr->SetPosition(cVector3(m_XMovementParams.m_From, m_YMovementParams.m_From, 0.0f));
  }
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
    m_ElapsedTime += deltaTime;
    shared_ptr<cTweenManager> pTweenManger =
        (MakeStrongPtr<cTweenManager>(cServiceLocator::GetInstance()->GetService<cTweenManager>()));
    if (pTweenManger != NULL && m_ElapsedTime <= m_XMovementParams.m_Duration)
    {
      cVector3 position;

      position.x = pTweenManger->Interpolate<float>(m_ElapsedTime, m_XMovementParams);
      position.y = pTweenManger->Interpolate<float>(m_ElapsedTime, m_YMovementParams);
      m_pTransformComponentStrongPtr->SetPosition(position);
    }
    if (m_ElapsedTime > (m_XMovementParams.m_Duration + 0.5f))
    {
      VOnRestart();
    }
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
    IncrementXTransition();
    VOnRestart();
  }
  else if (IKeyboardController::Instance()->VIsKeyPressed('w') || IKeyboardController::Instance()->VIsKeyPressed('W'))
  {
    IncrementXEasingType();
    VOnRestart();
  }
  else if (IKeyboardController::Instance()->VIsKeyPressed('a') || IKeyboardController::Instance()->VIsKeyPressed('A'))
  {
    IncrementYTransition();
    VOnRestart();
  }
  else if (IKeyboardController::Instance()->VIsKeyPressed('s') || IKeyboardController::Instance()->VIsKeyPressed('S'))
  {
    IncrementYEasingType();
    VOnRestart();
  }
  else if (IKeyboardController::Instance()->VIsKeyPressed('z') || IKeyboardController::Instance()->VIsKeyPressed('Z'))
  {
    IncrementXTransition();
    IncrementYTransition();
    VOnRestart();
  }
  else if (IKeyboardController::Instance()->VIsKeyPressed('x') || IKeyboardController::Instance()->VIsKeyPressed('X'))
  {
    IncrementXEasingType();
    IncrementYEasingType();
    VOnRestart();
  }
  else if (IKeyboardController::Instance()->VIsKeyPressed('r') || IKeyboardController::Instance()->VIsKeyPressed('R'))
  {
    m_XMovementParams.m_Transition = EasingTransitions::Linear;
    m_XMovementParams.m_Version = EasingType::EaseIn;
    m_YMovementParams.m_Transition = EasingTransitions::Linear;
    m_YMovementParams.m_Version = EasingType::EaseIn;

    VOnRestart();
  }
#endif
}

//  *******************************************************************************************************************
cString cBall::GetCurrentXTransition()
{
  return EasingTransitions::ToString(m_XMovementParams.m_Transition);
}

//  *******************************************************************************************************************
cString cBall::GetCurrentXEasingType()
{
  return EasingType::ToString(m_XMovementParams.m_Version);
}

//  *******************************************************************************************************************
cString cBall::GetCurrentYTransition()
{
  return EasingTransitions::ToString(m_XMovementParams.m_Transition);
}

//  *******************************************************************************************************************
cString cBall::GetCurrentYEasingType()
{
  return EasingType::ToString(m_XMovementParams.m_Version);
}

//  *******************************************************************************************************************
bool cBall::IncrementXTransition()
{
  m_XMovementParams.m_Transition = (EasingTransitions::Enum)(static_cast<int>(m_XMovementParams.m_Transition) + 1);
  if (m_XMovementParams.m_Transition == EasingTransitions::Total)
  {
    m_XMovementParams.m_Transition = EasingTransitions::Linear;
    return true;
  }
  return false;
}

//  *******************************************************************************************************************
bool cBall::IncrementXEasingType()
{
  m_XMovementParams.m_Version = (EasingType::Enum)(static_cast<int>(m_XMovementParams.m_Version) + 1);
  if (m_XMovementParams.m_Version == EasingType::Total)
  {
    m_XMovementParams.m_Version = EasingType::EaseIn;
    return true;
  }
  return false;
}

//  *******************************************************************************************************************
bool cBall::IncrementYTransition()
{
  m_YMovementParams.m_Transition = (EasingTransitions::Enum)(static_cast<int>(m_YMovementParams.m_Transition) + 1);
  if (m_YMovementParams.m_Transition == EasingTransitions::Total)
  {
    m_YMovementParams.m_Transition = EasingTransitions::Linear;
    return true;
  }
  return false;
}

//  *******************************************************************************************************************
bool cBall::IncrementYEasingType()
{
  m_YMovementParams.m_Version = (EasingType::Enum)(static_cast<int>(m_YMovementParams.m_Version) + 1);
  if (m_YMovementParams.m_Version == EasingType::Total)
  {
    m_YMovementParams.m_Version = EasingType::EaseIn;
    return true;
  }
  return false;
}

//  *******************************************************************************************************************
void cBall::ReInit()
{
  m_ElapsedTime = 0.0f;
  if (m_pTransformComponentStrongPtr != nullptr)
  {
    m_pTransformComponentStrongPtr->SetPosition(cVector3(m_XMovementParams.m_From, m_YMovementParams.m_From, 0.0f));
  }
}
