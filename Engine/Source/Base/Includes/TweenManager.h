//  *******************************************************************************************************************
//  TweenManager   version:  1.0   Ankur Sheel  date: 2015/02/02
//  *******************************************************************************************************************
//  Equations from http://gizma.com/easing
//  *******************************************************************************************************************
#ifndef TWEENMANAGER_HXX__
#define TWEENMANAGER_HXX__

#include "BaseDefines.h"

namespace Base
{
  class cBaseEasing;
}

namespace Base
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
    BASE_API static cString ToString(Enum transition);
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

    BASE_API static cString ToString(Enum type);
  };

  template <class T>
  class cTweenParameters
  {
  public:
    EasingTransitions::Enum m_Transition;
    EasingType::Enum m_Version;
    float m_Duration;
    T m_From;
    T m_To;

    cTweenParameters(EasingTransitions::Enum transition, EasingType::Enum version, const T& from, const T& to,
                     float duration)
      : m_Transition(transition)
      , m_Version(version)
      , m_Duration(duration)
      , m_To(to)
      , m_From(from)
    {
    }

    cTweenParameters()
      : m_Transition(EasingTransitions::Linear)
      , m_Version(EasingType::EaseIn)
      , m_Duration(0.0f)
    {
    }
  };

  class cTweenManager
  {
  public:
    ~cTweenManager();
    template <class T>
    const T& Interpolate(float time, const cTweenParameters<T>& params);

    BASE_API static void CreateAsService();

  private:
    BASE_API float GetFactor(EasingTransitions::Enum transition, EasingType::Enum version, float time, float duration);

  private:
    std::map<EasingTransitions::Enum, cBaseEasing*> m_EasingFunctions;
    cTweenManager();
  };

  //  *******************************************************************************************************************
  template <class T>
  const T& cTweenManager::Interpolate(float time, const cTweenParameters<T>& params)
  {
    float alpha = GetFactor(params.m_Transition, params.m_Version, time, params.m_Duration);
    const T& val = (params.m_From * (1.0f - alpha)) + (params.m_To * alpha);
    // const T& val = ((to - from) * alpha) + from;
    return val;
  }
}  // namespace Base
#endif  // TWEENMANAGER_HXX__
