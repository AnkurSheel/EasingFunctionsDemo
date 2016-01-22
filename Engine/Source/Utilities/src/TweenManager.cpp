#include "stdafx.h"
#include "TweenManager.h"
#include "EasingFunctions.h"
#include "TweenParameters.h"

using namespace Base;
using namespace Utilities;

const cHashedString cTweenManager::m_Type = cHashedString("tweenmanager");


//  *******************************************************************************************************************
cTweenManager::cTweenManager()
{
	m_EasingFunctions[EasingTransitions::Linear] = DEBUG_NEW cLinearEasing();
	m_EasingFunctions[EasingTransitions::Sine] = DEBUG_NEW cSineEasing();
	m_EasingFunctions[EasingTransitions::Quadratic] = DEBUG_NEW cQuadraticEasing();
	m_EasingFunctions[EasingTransitions::Cubic] = DEBUG_NEW cCubicEasing();
	m_EasingFunctions[EasingTransitions::Quartic] = DEBUG_NEW cQuarticEasing();
	m_EasingFunctions[EasingTransitions::Quintic] = DEBUG_NEW cQuinticEasing();
	m_EasingFunctions[EasingTransitions::Exponential] = DEBUG_NEW cExponentialEasing();
	m_EasingFunctions[EasingTransitions::Circular] = DEBUG_NEW cCircularEasing();
	m_EasingFunctions[EasingTransitions::Elastic] = DEBUG_NEW cElasticEasing();
	m_EasingFunctions[EasingTransitions::Back] = DEBUG_NEW cBackEasing();
	m_EasingFunctions[EasingTransitions::Bounce] = DEBUG_NEW cBounceEasing();
}

//  *******************************************************************************************************************
cTweenManager::~cTweenManager()
{
	for (auto iter = m_EasingFunctions.begin(); iter != m_EasingFunctions.end(); iter++)
	{
		SafeDelete(&(iter->second));
	}
	m_EasingFunctions.clear();
}

//  *******************************************************************************************************************
void cTweenManager::AddTween(const cTweenParameters& tween)
{
	for (auto tweenIter = m_Tweens.begin(); tweenIter != m_Tweens.end(); tweenIter++)
	{
		if ((tween == (*tweenIter)))
		{
			SP_ASSERT(!(tween == (*tweenIter))).SetCustomMessage("Trying to add duplicate tween");
			return;
		}
	}
	m_Tweens.push_back(tween);
}

//  *******************************************************************************************************************
void cTweenManager::RemoveTween(const cTweenParameters& tween)
{
	for (auto tweenIter = m_Tweens.begin(); tweenIter != m_Tweens.end(); tweenIter++)
	{
		if ((tween == (*tweenIter)))
		{
			m_Tweens.erase(tweenIter);
			break;
		}
	}
}

//  *******************************************************************************************************************
float cTweenManager::GetFactor(const cTweenParameters& tween)
{
	auto findIt = m_EasingFunctions.find(tween.m_TransitionType);

	SP_ASSERT_WARNING(findIt != m_EasingFunctions.end()).SetCustomMessage("Could not find tweening function for "
		+ EasingTransitions::ToString(tween.m_TransitionType) + " " + EasingType::ToString(tween.m_EasingType));

	if (findIt != m_EasingFunctions.end())
	{
		cBaseEasing * pFunction = findIt->second;
		if (pFunction != NULL)
		{
			float elapsedTime = tween.m_ElapsedTime - tween.m_Delay;
			if (tween.m_EasingType == EasingType::EaseIn)
			{
				return pFunction->VEaseIn(elapsedTime, tween.m_Duration, tween.m_extraParams);
			}
			else if (tween.m_EasingType == EasingType::EaseOut)
			{
				return pFunction->VEaseOut(elapsedTime, tween.m_Duration, tween.m_extraParams);
			}
			else if (tween.m_EasingType == EasingType::EaseInOut)
			{
				return pFunction->VEaseInOut(elapsedTime, tween.m_Duration, tween.m_extraParams);
			}
			else if (tween.m_EasingType == EasingType::EaseOutIn)
			{
				return pFunction->VEaseOutIn(elapsedTime, tween.m_Duration, tween.m_extraParams);
			}
			else
			{
				SP_ASSERT_WARNING(false).SetCustomMessage("Could not find tweening function type for " + EasingType::ToString(tween.m_EasingType));
			}
		}
	}
	return 0.0f;
}

//  *******************************************************************************************************************
void cTweenManager::VUpdate(const float deltaTime)
{
	cProcess::VUpdate(deltaTime);

	auto iter = m_Tweens.begin();

	while (iter != m_Tweens.end())
	{
		cTweenParameters & tween = (*iter);
		tween.m_ElapsedTime += deltaTime;
		if (tween.m_ElapsedTime < tween.m_Delay)
		{
			iter++;
			continue;
		}

		if (tween.m_ElapsedTime <= tween.m_Duration + tween.m_Delay)
		{
			float alpha = GetFactor(tween);
			tween.Interpolate(alpha);
			iter++;
		}
		else
		{
			if (tween.m_CallbackFunction != NULL)
			{
				tween.m_CallbackFunction(tween);
			}
			if (tween.m_RepeatCount.IsValid() )
			{
				tween.m_ElapsedTime = 0.0f;
				tween.m_RepeatCount.GetValue()--;
			}
			if (tween.m_RepeatCount.IsInvalid() || tween.m_RepeatCount.GetValue() == 0)
			{
				iter = m_Tweens.erase(iter);
			}
		}
	}
}

//  *******************************************************************************************************************
void cTweenManager::CreateAsService()
{
	cServiceLocator::GetInstance()->Register<cTweenManager>(shared_ptr<cTweenManager>(DEBUG_NEW cTweenManager()));
}
