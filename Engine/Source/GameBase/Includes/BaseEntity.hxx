//  *******************************************************************************************************************
//  BaseEntity   version:  1.0   Ankur Sheel  date: 2012/07/31
//  *******************************************************************************************************************
//  purpose:
//  *******************************************************************************************************************
#ifndef BaseEntity_hxx__
#define BaseEntity_hxx__

#include "GameBaseDefines.h"

namespace AI
{
  struct Telegram;
}

namespace GameBase
{
  class IBaseComponent;
}

namespace GameBase
{
  class IBaseEntity
  {
  public:
    typedef std::list<shared_ptr<IBaseComponent> > ComponentList;

  public:
    virtual ~IBaseEntity() {}
    virtual void VInitialize() = 0;
    virtual void VOnUpdate(const float deltaTime) = 0;
    virtual void VOnRestart() = 0;
    virtual void VCleanup() = 0;
    virtual bool VOnHandleMessage(const AI::Telegram& telegram) = 0;
    virtual void VHandleInput(const float inDeltaTime) = 0;
    virtual Base::cHashedString VGetEntityType() const = 0;
    virtual UINT64 VGetHashedID() const = 0;
    virtual int VGetEntityID() const = 0;
    virtual void VOnCollided(shared_ptr<IBaseEntity> const pEntityCollider) = 0;
    virtual void VOnEnteredTrigger(shared_ptr<IBaseEntity> const pTrigger) = 0;
    virtual void VOnTriggered(shared_ptr<IBaseEntity> const pEntityCollider) = 0;
    virtual Base::cString VToXml() = 0;
    virtual void VSetActive(const bool active) = 0;
    virtual bool VIsActive() const = 0;
  };
}  // namespace GameBase
#endif  // BaseEntity_hxx__
