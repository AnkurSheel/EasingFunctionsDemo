//  *******************************************************************************************************************
//  TweenManager   version:  1.0   Ankur Sheel  date: 2015/02/02
//  *******************************************************************************************************************
//  Equations from http:// gizma.com/easing
//  reference : https:// code.google.com/p/cpptweener/
//  *******************************************************************************************************************
#ifndef TWEENMANAGER_HXX__
#define TWEENMANAGER_HXX__

#include "UtilitiesDefines.h"
#include "Process.h"
#include "TweenStructures.h"

namespace Utilities
{
	class cBaseEasing;
	class cTweenParameters;
}

namespace Utilities
{
	class cTweenManager
		: public cProcess
	{
	public:
		cTweenManager();
		~cTweenManager();
		UTILITIES_API void AddTween(const cTweenParameters& tween);
		UTILITIES_API void RemoveTween(const cTweenParameters& tween);
		UTILITIES_API static void CreateAsService();
		const Base::cHashedString & VGetType() const OVERRIDE { return m_Type; }

	private:
		void VUpdate(const float deltaTime) OVERRIDE;
		float GetFactor(const cTweenParameters& tween);

	private:
		std::map<EasingTransitions::Enum, cBaseEasing*> m_EasingFunctions;
		std::list<cTweenParameters> m_Tweens;
		static const Base::cHashedString	m_Type;
	};
}  // namespace Utilities
#endif  // TWEENMANAGER_HXX__
