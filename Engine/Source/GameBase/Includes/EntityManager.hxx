//  *******************************************************************************************************************
//  EntityManager   version:  1.0   Ankur Sheel  date: 2012/07/31
//  *******************************************************************************************************************
// 
//  *******************************************************************************************************************
#ifndef EntityManager_hxx__
#define EntityManager_hxx__

#include "GameBaseDefines.h"

namespace GameBase
{
	class IBaseEntity;
	class IBaseComponent;
	class cEntityFactory;
	class cComponentFactory;
}

namespace GameBase
{
	class IEntityManager
	{
	public:
		typedef std::list<shared_ptr<IBaseEntity> > EntityList;
		typedef std::map<int, shared_ptr<IBaseEntity> const > EntityMap;

	public:
		virtual ~IEntityManager(){}
		virtual void VSetAndInitializeEntityFactory(std::unique_ptr<cEntityFactory> pEntityFactory) = 0;
		virtual void VSetAndInitializeComponentFactory(std::unique_ptr<cComponentFactory> pComponentFactory) = 0;
		virtual void VAddEntity(shared_ptr<IBaseEntity> const pNewEntity) = 0;
		virtual shared_ptr<IBaseEntity> const VAddEntity(const Base::cString & Type) = 0;
		virtual void VDeleteEntity(shared_ptr<IBaseEntity> const pNewEntity) = 0;
		virtual shared_ptr<IBaseEntity> const VGetEntityFromID(const int iID) const = 0;
		virtual Base::cString VGetEntityNameFromID(const int iID) const  = 0;
		virtual Base::cString VGetEntityName(shared_ptr<IBaseEntity> const pEntity) const = 0;
		virtual int VGetEntityID(const IBaseEntity * const pEntity) const = 0;
		virtual shared_ptr<IBaseEntity> const VGetEntityOfType(const Base::cHashedString & Type) = 0;
		virtual shared_ptr<IBaseComponent> VGetComponent(shared_ptr<IBaseEntity> const pEntity, const Base::cHashedString & ComponentName) const = 0;
		virtual shared_ptr<IBaseEntity> const VGetFirstEntityWithComponent(const Base::cHashedString & Component) = 0;
		virtual void VGetEntitiesWithComponent(const Base::cHashedString & Component, EntityList & entities) = 0;
		virtual void VAddComponent(shared_ptr<IBaseEntity> const pEntity, shared_ptr<IBaseComponent> pComponent) = 0;
		virtual void VRemoveComponent(shared_ptr<IBaseEntity> const pEntity,	const UINT64 ComponentID) = 0;
		virtual void VUpdate(const float deltaTime) = 0;
		virtual void VDeleteAllEntities() = 0;
		virtual int VGetNumberOfEntities() const = 0;
		virtual const EntityMap& VGetEntityMap() const = 0;

		GAMEBASE_API static void CreateAsService();
	};
}

#endif  // EntityManager_hxx__