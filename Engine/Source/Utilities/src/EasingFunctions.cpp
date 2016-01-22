#include "stdafx.h"
#include "EasingFunctions.h"
#include "TweenStructures.h"

using namespace Utilities;
using namespace Base;

//  *******************************************************************************************************************
float cBaseEasing::VEaseOut(float time, float duration, const stExtraTweenParams& param)
{
	return (1.0f - VEaseIn(1 - time / duration, 1.0f, param));
}

//  *******************************************************************************************************************
float cBaseEasing::VEaseInOut(float time, float duration, const stExtraTweenParams& param)
{
	time /= duration;
	if (time < 0.5f)
	{
		return VEaseIn(time * 2.0f, 1.0f, param) * 0.5f;
	}
	return (1 - VEaseIn((1 - time) * 2.0f, 1.0f, param) * 0.5f);
}

//  *******************************************************************************************************************
float cBaseEasing::VEaseOutIn(float time, float duration, const stExtraTweenParams& param)
{
	time /= duration;
	if (time < 0.5f)
	{
		return VEaseOut(time * 2.0f, 1.0f, param) * 0.5f;
	}
	return (1 - VEaseOut((1 - time) * 2.0f, 1.0f, param) * 0.5f);
}

//  *******************************************************************************************************************
float cLinearEasing::VEaseIn(float time, float duration, const stExtraTweenParams& param)
{
	return time / duration;
}

//  *******************************************************************************************************************
float cSineEasing::VEaseIn(float time, float duration, const stExtraTweenParams& param)
{
	return -(cos(time / duration * HalfPi)) + 1;
}

//  *******************************************************************************************************************
float cQuadraticEasing::VEaseIn(float time, float duration, const stExtraTweenParams& param)
{
	time /= duration;
	return (time * time);
}

//  *******************************************************************************************************************
float cCubicEasing::VEaseIn(float time, float duration, const stExtraTweenParams& param)
{
	time /= duration;
	return (time * time * time);
}

//  *******************************************************************************************************************
float cQuarticEasing::VEaseIn(float time, float duration, const stExtraTweenParams& param)
{
	time /= duration;
	return (time * time * time * time);
}

//  *******************************************************************************************************************
float cQuinticEasing::VEaseIn(float time, float duration, const stExtraTweenParams& param)
{
	time /= duration;
	return (time * time * time * time * time);
}

//  *******************************************************************************************************************
float cExponentialEasing::VEaseIn(float time, float duration, const stExtraTweenParams& param)
{
	return (pow(2, (10 * (time / duration - 1))));
}

//  *******************************************************************************************************************
float cCircularEasing::VEaseIn(float time, float duration, const stExtraTweenParams& param)
{
	time /= duration;
	return -(sqrt(1 - time * time) - 1);
}

//  *******************************************************************************************************************
float cElasticEasing::VEaseIn(float time, float duration, const stExtraTweenParams& param)
{
	time /= duration;
	time--;
	return -(pow(2, 10 * time) * sin((time - param.periodElasticTween * 0.25f) * TwoPi / param.periodElasticTween));
}

//  *******************************************************************************************************************
float cBackEasing::VEaseIn(float time, float duration, const stExtraTweenParams& param)
{
	time /= duration;
	return time * time * ((param.overshootBackTween + 1) * time - param.overshootBackTween);
}

//  *******************************************************************************************************************
float cBounceEasing::VEaseIn(float time, float duration, const stExtraTweenParams& param)
{
	return (1.0f - VEaseOut(duration - time, duration, param));
}

//  *******************************************************************************************************************
float cBounceEasing::VEaseOut(float time, float duration, const stExtraTweenParams& param)
{
	time /= duration;
	if (time < (1/2.75f))
	{
		return (7.5625f * time * time);
	}
	else if (time < (2/2.75f))
	{
		time -= (1.5f/2.75f);
		return (7.5625f * time * time + .75f);
	}
	else if (time < (2.5/2.75))
	{
		time -= (2.25f/2.75f);
		return (7.5625f * time * time + .9375f);
	}
	else
	{
		time -= (2.625f/2.75f);
		return (7.5625f * time * time + .984375f);
	}
}