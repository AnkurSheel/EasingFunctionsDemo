//  *******************************************************************************************************************
//  CollisionInfo version:  1.0   Ankur Sheel  date: 2013/06/23
//  *******************************************************************************************************************
//  purpose:
//  *******************************************************************************************************************
#ifndef CollisionInfo_h
#define CollisionInfo_h

#include "vector3.h"

namespace Physics
{
  class cRigidBody;
}

namespace Physics
{
  class cCollisionInfo
  {
  public:
    cCollisionInfo(cRigidBody* const pBodyA, cRigidBody* const pBodyB);
    ~cCollisionInfo();
    cCollisionInfo(const cCollisionInfo& other);
    bool GetCollided() const { return m_Collided; }
    const cRigidBody* const GetBodyA() const { return m_pBodyA; }
    const cRigidBody* const GetBodyB() const { return m_pBodyB; }
    void Solve();
    void ApplyImpulse();
    void ApplyPositionCorrection();

  private:
    cCollisionInfo& operator=(const cCollisionInfo& other);
    void InfiniteMassCorrection();
    void AABBvsAABB();
    void CircleVsAABB();
    void AABBVsCircle();
    void CircleVsCircle();

  private:
    Base::cVector3 m_Normal;
    cRigidBody* m_pBodyA;
    cRigidBody* m_pBodyB;
    float m_PenetrationDistance;
    bool m_Collided;
  };
}  // namespace Physics
#endif  // CollisionInfo_h
