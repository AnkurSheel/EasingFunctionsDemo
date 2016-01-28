//  *******************************************************************************************************************
//  BaseEntity   version:  1.0   Ankur Sheel  date: 2012/07/30
//  *******************************************************************************************************************
//  purpose:
//  *******************************************************************************************************************
#ifndef BaseEntity_h__
#define BaseEntity_h__

#include "BaseEntity.hxx"
#include "GameBaseDefines.h"

namespace GameBase
{
  class IBaseComponent;
  class cComponentDef;
  class cTransformComponent;
}

namespace GameBase
{
  typedef std::map<UINT64, shared_ptr<IBaseComponent> > ComponentMap;
  class cEntityDef : public Base::cNonCopyable
  {
  public:
    GAMEBASE_API cEntityDef();
    GAMEBASE_API virtual ~cEntityDef();

  public:
    bool m_IsCollidable;
    Base::cString m_Type;
    ComponentMap m_Components;

  private:
    void Cleanup();
  };

  class cBaseEntity : public IBaseEntity, public std::enable_shared_from_this<cBaseEntity>
  {
  public:
    GAMEBASE_API cBaseEntity();
    explicit GAMEBASE_API cBaseEntity(const int ID);
    GAMEBASE_API virtual ~cBaseEntity();
    GAMEBASE_API void VInitialize() OVERRIDE;
    GAMEBASE_API void VOnCollided(shared_ptr<IBaseEntity> const pEntityCollider) OVERRIDE {}
    GAMEBASE_API void VOnEnteredTrigger(shared_ptr<IBaseEntity> const pTrigger) OVERRIDE {}
    GAMEBASE_API void VOnTriggered(shared_ptr<IBaseEntity> const pEntityCollider) OVERRIDE {}
    GAMEBASE_API void VOnRestart() OVERRIDE;
    GAMEBASE_API void VCleanup() OVERRIDE;
    GAMEBASE_API bool VOnHandleMessage(const AI::Telegram& telegram) OVERRIDE;
    GAMEBASE_API void VHandleInput(const float inDeltaTime) OVERRIDE;
    GAMEBASE_API Base::cString VToXml();

    GAMEBASE_API void VOnInitialized(bool reInitialize);
    GAMEBASE_API void AddComponent(shared_ptr<IBaseComponent> pComponent);
    GAMEBASE_API void RemoveComponent(const UINT64 ComponentID);
    GAMEBASE_API void GetAllComponents(ComponentList& components) const;
    GAMEBASE_API shared_ptr<IBaseComponent> GetComponent(const Base::cHashedString& ComponentName) const;
    GAMEBASE_API shared_ptr<IBaseComponent> GetComponent(const UINT64 ComponentID) const;

    GAMEBASE_API void VSetActive(const bool active) OVERRIDE;
    GAMEBASE_API bool VIsActive() const OVERRIDE { return m_Active; };
    GAMEBASE_API int VGetEntityID() const OVERRIDE { return m_ID; }
  private:
    void SetEntityID(const int ID);

  protected:
    weak_ptr<cTransformComponent> m_pTransFormComponent;

  private:
    int m_ID;
    bool m_Active;
    ComponentMap m_Components;
    static int m_sNextValidID;
  };
}  // namespace GameBase
#endif  // BaseEntity_h__
