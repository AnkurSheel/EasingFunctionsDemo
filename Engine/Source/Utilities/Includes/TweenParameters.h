//  *******************************************************************************************************************
//  TweenParameters   version:  1.0   Ankur Sheel  date: 2015/04/15
//  *******************************************************************************************************************
//
//  *******************************************************************************************************************

#ifndef TWEENPARAMETERS_H__
#define TWEENPARAMETERS_H__

#include "UtilitiesDefines.h"
#include "TweenStructures.h"
#include "Optional.h"

namespace Utilities
{
	class cTweenParameters
	{
	public:
		typedef function<void (const cTweenParameters &)> TweenCallBackFn;

		UTILITIES_API cTweenParameters(float delay, float duration, EasingTransitions::Enum transition, EasingType::Enum version,
			stExtraTweenParams params = stExtraTweenParams(), const Base::tOptional<int>& repeatCount = Base::tOptionalEmpty());
		UTILITIES_API void AddProperty(float* propValue, float finalValue);
		UTILITIES_API void RegisterCallBack(TweenCallBackFn fnCallback);
		UTILITIES_API void DeRegisterCallBack();
		bool operator == (const cTweenParameters& tween) const;
		void Interpolate(const float alpha);

	public:
		EasingTransitions::Enum m_TransitionType;
		EasingType::Enum m_EasingType;
		Base::tOptional<int> m_RepeatCount;
		float m_Duration;
		float m_ElapsedTime;
		float m_Delay;
		TweenCallBackFn m_CallbackFunction;
		stExtraTweenParams m_extraParams;
		std::vector<stTweenProperty> m_Properties;
	};
}  // namespace Utilities
#endif  // TWEENPARAMETERS_H__
