//  *******************************************************************************************************************
//  TweenStructures   version:  1.0   Ankur Sheel  date: 2015/04/16
//  *******************************************************************************************************************
//
//  *******************************************************************************************************************
#ifndef TWEENSTRUCTURES_H__
#define TWEENSTRUCTURES_H__

namespace Utilities
{

	struct EasingTransitions
	{
		enum Enum
		{
			Linear,
			Sine,
			Quadratic,
			Cubic,
			Quartic,
			Quintic,
			Exponential,
			Circular,
			Elastic,
			Back,
			Bounce,
			Total
		};

		UTILITIES_API static Base::cString ToString(Enum transition);
	};

	struct EasingType
	{
		enum Enum
		{
			EaseIn,
			EaseOut,
			EaseInOut,
			EaseOutIn,
			Total,
		};

		UTILITIES_API static Base::cString ToString(Enum type);
	};

	struct stTweenProperty
	{
		float *ptrValue;
		float initialValue;
		float finalValue;
	};

	struct stExtraTweenParams
	{
		float overshootBackTween;  // higher s means greater overshoot (0 produces cubic easing with no overshoot, and the default value of 1.70158 produces an overshoot of 10 percent).
		float periodElasticTween;

		stExtraTweenParams()
			: overshootBackTween(1.70158f)
			, periodElasticTween(0.3f)
		{
		}
	};
}  // namespace Utilities
#endif  // TWEENSTRUCTURES_H__
