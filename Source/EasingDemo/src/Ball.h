//  *******************************************************************************************************************
//  Ball   version:  1.0   Ankur Sheel  date: 2013/05/13
//  *******************************************************************************************************************
//  purpose:
//  *******************************************************************************************************************
#ifndef Ball_h__
#define Ball_h__

#include "BaseEntity.h"
#include "Vector3.h"
#include "TweenParameters.h"

namespace Base
{
  class cVector3;

  template <class BaseType, class SubType>
  shared_ptr<BaseType> GenericObjectCreationFunction();
}

namespace Utilities
{
  class cTweenParameters;
}
namespace GameBase
{
  class cPhysicsComponent;
  class cTransformComponent;
}

class cBall : public GameBase::cBaseEntity
{
public:
  ~cBall();
  Base::cString GetCurrentXTransition();
  Base::cString GetCurrentXEasingType();
  Base::cString GetCurrentYTransition();
  Base::cString GetCurrentYEasingType();

  void OnKeyPressed(int key);

public:
  static const Base::cHashedString m_EntityType;  ///< The component name

private:
  cBall();
  void VInitialize() OVERRIDE;
  void VOnUpdate(const float deltaTime) OVERRIDE;
  void VCleanup() OVERRIDE;
  void VOnRestart() OVERRIDE;
  Base::cHashedString VGetEntityType() const OVERRIDE { return m_EntityType; }
  UINT64 VGetHashedID() const OVERRIDE { return m_EntityType.GetHash(); }
  bool IncrementYEasingType();
  bool IncrementYTransition();
  bool IncrementXEasingType();
  bool IncrementXTransition();
  void RemoveExistingTweens();
  void ReInit();
  void OnTweenCompleted(const Utilities::cTweenParameters& params);

  cBall* CastToBall();

private:
  shared_ptr<GameBase::cTransformComponent> m_pTransformComponentStrongPtr;
  Utilities::cTweenParameters m_XTweenParams;
  Utilities::cTweenParameters m_YTweenParams;
  Base::cVector3 m_Position;
  Base::cVector3 m_Scale;
  Base::cVector3 m_InitialPosition;
  Base::cVector3 m_FinalPosition;
  Base::cVector3 m_InitialScale;
  Base::cVector3 m_FinalScale;
  bool m_bTweenCompleted;

private:
  template <class BaseType, class SubType>
  friend shared_ptr<BaseType> Base::GenericObjectCreationFunction();
};
#endif  // Ball_h__
