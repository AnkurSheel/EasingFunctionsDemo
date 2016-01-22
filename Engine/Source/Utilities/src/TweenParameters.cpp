#include "stdafx.h"
#include "TweenParameters.h"

using namespace Base;
using namespace Utilities;

//  *******************************************************************************************************************
cTweenParameters::cTweenParameters(float delay, float duration, EasingTransitions::Enum transition
	, EasingType::Enum version, stExtraTweenParams params/* = stExtraTweenParams()*/, const Base::tOptional<int>& repeatCount/* = Base::tOptionalEmpty()*/)
	: m_TransitionType(transition)
	, m_EasingType(version)
	, m_Duration(duration)
	, m_Delay(delay)
	, m_ElapsedTime(0.0f)
	, m_extraParams(params)
	, m_RepeatCount(repeatCount)
{
}

//  *******************************************************************************************************************
void cTweenParameters::AddProperty(float * propValue, float finalValue)
{
	stTweenProperty prop = {propValue, *propValue, finalValue};
	m_Properties.push_back(prop);
}

//  *******************************************************************************************************************
bool cTweenParameters::operator == (const cTweenParameters& tween) const
{
	if (m_Duration == tween.m_Duration
		&& m_TransitionType == tween.m_TransitionType
		&& m_EasingType == tween.m_EasingType
		&& m_Properties.size() == tween.m_Properties.size())
	{
		for (unsigned int i = 0; i < tween.m_Properties.size(); i++)
		{
			if (m_Properties[i].ptrValue != tween.m_Properties[i].ptrValue
				|| m_Properties[i].initialValue != tween.m_Properties[i].initialValue
				|| m_Properties[i].finalValue != tween.m_Properties[i].finalValue)
			{
				return false;
			}
		}
		return true;
	}
	return false;
}

//  *******************************************************************************************************************
void cTweenParameters::Interpolate(const float alpha)
{
	for (auto iter = m_Properties.begin(); iter != m_Properties.end(); iter++)
	{
		stTweenProperty & prop = (*iter);
		*(prop.ptrValue) = (prop.initialValue * (1.0f - alpha)) + (prop.finalValue * alpha);
		// const T& val = ((to - from) * alpha) + from;
	}
}

//  *******************************************************************************************************************
void cTweenParameters::RegisterCallBack(TweenCallBackFn fnCallback)
{
	m_CallbackFunction = fnCallback;
}

//  *******************************************************************************************************************
void cTweenParameters::DeRegisterCallBack()
{
	m_CallbackFunction = nullptr;
}
