//  *******************************************************************************************************************
//  EntityManager   version:  1.0   Ankur Sheel  date: 2012/07/27
//  *******************************************************************************************************************
//
//  *******************************************************************************************************************
#ifndef EntityManager_h__
#define EntityManager_h__

#include "EntityManager.hxx"

namespace GameBase
{
  class IBaseEntity;
  class IBaseComponent;
}

namespace GameBase
{
  class cEntityManager : public IEntityManager, public Base::cNonCopyable
  {
  public:
    ~cEntityManager();

  private:
    typedef std::map<UINT64, EntityList> EntityComponentMap;

  private:
    void VSetAndInitializeEntityFactory(std::unique_ptr<cEntityFactory> pEntityFactory) OVERRIDE;
    void VSetAndInitializeComponentFactory(std::unique_ptr<cComponentFactory> pComponentFactory);
    void VAddEntity(shared_ptr<IBaseEntity> const pNewEntity) OVERRIDE;
    shared_ptr<IBaseEntity> const VAddEntity(const Base::cString& Type) OVERRIDE;
    void VDeleteEntity(shared_ptr<IBaseEntity> const pNewEntity) OVERRIDE;
    void VAddComponent(shared_ptr<IBaseEntity> const pEntity, shared_ptr<IBaseComponent> pComponent) OVERRIDE;
    void VRemoveComponent(shared_ptr<IBaseEntity> const pEntity, const UINT64 ComponentID) OVERRIDE;
    shared_ptr<IBaseEntity> const VGetFirstEntityWithComponent(const Base::cHashedString& Component) OVERRIDE;
    void VGetEntitiesWithComponent(const Base::cHashedString& Component, EntityList& entities) OVERRIDE;
    shared_ptr<IBaseEntity> const VGetEntityFromID(const int iID) const OVERRIDE;
    Base::cString VGetEntityNameFromID(const int iID) const OVERRIDE;
    Base::cString VGetEntityName(shared_ptr<IBaseEntity> const pEntity) const OVERRIDE;
    shared_ptr<IBaseEntity> const VGetEntityOfType(const Base::cHashedString& Type) OVERRIDE;
    shared_ptr<IBaseComponent> VGetComponent(shared_ptr<IBaseEntity> const,
                                             const Base::cHashedString& ComponentName) const OVERRIDE;
    int VGetEntityID(const IBaseEntity* const pEntity) const OVERRIDE;
    void VUpdate(const float deltaTime) OVERRIDE;
    void VDeleteAllEntities() OVERRIDE;

    int VGetNumberOfEntities() const OVERRIDE { return m_EntityMap.size(); }
    const EntityMap& VGetEntityMap() const OVERRIDE { return m_EntityMap; }
    void InitializeEntity(shared_ptr<IBaseEntity> const pEntity);
    void RemoveDeletedEntities();
    void Cleanup();

    cEntityManager();

  private:
    std::unique_ptr<cEntityFactory> m_pEntityFactory;
    std::unique_ptr<cComponentFactory> m_pComponentFactory;
    EntityMap m_EntityMap;
    EntityComponentMap m_ComponentMap;
    EntityList m_DeletedEntities;

  private:
    friend void IEntityManager::CreateAsService();
  };
}  // namespace GameBase
#endif  // EntityManager_h__
