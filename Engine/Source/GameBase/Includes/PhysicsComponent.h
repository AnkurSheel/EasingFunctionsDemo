//  *******************************************************************************************************************
//  PhysicsComponent version:  1.0   Ankur Sheel  date: 2013/05/17
//  *******************************************************************************************************************
//  purpose:
//  *******************************************************************************************************************
#ifndef PhysicsComponent_h__
#define PhysicsComponent_h__

#include "BaseComponent.h"
#include "GameBaseDefines.h"
#include "vector3.h"

namespace Base
{
  template <class BaseType, class SubType>
  shared_ptr<BaseType> GenericObjectCreationFunction();
}

namespace Physics
{
  class IRigidBody;
  struct stRigidBodyDef;
}

namespace GameBase
{
  class cPhysicsComponent : public cBaseComponent
  {
  public:
    ~cPhysicsComponent();
    void VInitialize(const Utilities::IXMLNode* const pXMLNode) OVERRIDE;
    void VOnAttached(shared_ptr<IBaseEntity> const pOwner) OVERRIDE;
    void VCleanup() OVERRIDE;
    shared_ptr<Utilities::IXMLNode> VGenerateXml() const OVERRIDE;

    GAMEBASE_API static Base::cHashedString GetName() { return m_Name; }
    void Initialize(const Base::cVector3& position, const Base::cVector3& rotation, const Base::cVector3& size);
    void ReInitialize(const Base::cVector3& position, const Base::cVector3& rotation, const Base::cVector3& size);
    bool GetInitialized() const { return m_Initialized; }
    Base::cVector3 GetPosition() const;
    GAMEBASE_API void ApplyForce(const Base::cVector3& Direction);
    GAMEBASE_API void ApplyImpulse(const Base::cVector3& direction);
    void Update(const int deltaTime);
    void OnSizeUpdated();
    GAMEBASE_API void SetAsTrigger(bool isTrigger);
    GAMEBASE_API void GetBounds(Base::cVector3& minBound, Base::cVector3& maxBound);

  private:
    cPhysicsComponent();
    UINT64 VGetHashedID() const OVERRIDE { return m_Name.GetHash(); }
    void VSetActive(const bool active) OVERRIDE;
    void CalculateBounds(Base::cVector3& minBound, Base::cVector3& maxBound);

  private:
    Physics::IRigidBody* m_pRigidBody;
    shared_ptr<Physics::stRigidBodyDef> m_pDef;
    Base::cVector3 m_Direction;
    float m_Force;
    bool m_Initialized;
    bool m_ApplyForce;
    bool m_ApplyImpulse;
    static Base::cHashedString m_Name;  ///< The component name

  private:
    template <class BaseType, class SubType>
    friend shared_ptr<BaseType> Base::GenericObjectCreationFunction();
  };
}  // namespace GameBase
#endif  // PhysicsComponent_h__
