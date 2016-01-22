#include "stdafx.h"
#include "TweenStructures.h"

using namespace Utilities;
using namespace Base;

//  *******************************************************************************************************************
cString EasingTransitions::ToString(Enum transition)
{
	switch (transition)
	{
	case Linear:
		return "Linear";
		break;

	case Sine:
		return "Sinusoidal";
		break;

	case Quadratic:
		return "Quadratic";
		break;

	case Cubic:
		return "Cubic";
		break;

	case Quartic:
		return "Quartic";
		break;

	case Quintic:
		return "Quintic";
		break;

	case Exponential:
		return "Exponential";
		break;

	case Circular:
		return "Circular";
		break;

	case Elastic:
		return "Elastic";
		break;

	case Back:
		return "Back";
		break;

	case Bounce:
		return "Bounce";
		break;

	default:
		return "NA";
		break;
	}
}

//  *******************************************************************************************************************
cString EasingType::ToString(Enum type)
{
	switch (type)
	{
	case EaseIn:
		return "EaseIn";
		break;

	case EaseOut:
		return "EaseOut";
		break;

	case EaseInOut:
		return "EaseInOut";
		break;

	case EaseOutIn:
		return "EaseOutIn";
		break;

	default:
		return "NA";
		break;
	}
}
