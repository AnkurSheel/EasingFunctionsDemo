//  *******************************************************************************************************************
//  EnityManager   version:  1.0   Ankur Sheel  date: 2012/07/27
//  *******************************************************************************************************************
//  purpose:
//  *******************************************************************************************************************
#include "StdAfx.h"
#include "EntityManager.h"
#include "BaseEntity.h"
#include "BaseComponent.hxx"
#include "EntityFactory.h"
#include "ComponentFactory.h"

using namespace Utilities;
using namespace Base;
using namespace GameBase;
using namespace std;

//  *******************************************************************************************************************
cEntityManager::cEntityManager()
{
	m_EntityMap.clear();
	m_DeletedEntities.clear();
}

//  *******************************************************************************************************************
cEntityManager::~cEntityManager()
{
	Cleanup();
}

//  *******************************************************************************************************************
void GameBase::cEntityManager::VSetAndInitializeEntityFactory(std::unique_ptr<cEntityFactory> pEntityFactory)
{
	if (m_pEntityFactory == NULL && pEntityFactory != NULL)
	{
		m_pEntityFactory = std::move(pEntityFactory);
		m_pEntityFactory->VRegisterEntities();
	}
}

//  *******************************************************************************************************************
void GameBase::cEntityManager::VSetAndInitializeComponentFactory(std::unique_ptr<cComponentFactory> pComponentFactory)
{
	if (m_pComponentFactory == NULL && pComponentFactory != NULL)
	{
		m_pComponentFactory = std::move(pComponentFactory);
		m_pComponentFactory->VRegisterComponents();
	}
}
//  *******************************************************************************************************************
void cEntityManager::VAddEntity(shared_ptr<IBaseEntity>const pNewEntity)
{
	InitializeEntity(pNewEntity);
}

//  *******************************************************************************************************************
shared_ptr<IBaseEntity> const cEntityManager::VAddEntity(const cString & Type)
{
	SP_ASSERT_ERROR(m_pEntityFactory != NULL).SetCustomMessage("Entity Factory Not Created");

	shared_ptr<IBaseEntity> pEntity = m_pEntityFactory->VCreateEntity(cHashedString(cStringUtilities::GetInLowerCase(Type)));
	SP_ASSERT_ERROR(pEntity!= NULL).SetCustomMessage("Entity " + Type + " Not Created");

	InitializeEntity(pEntity);

	return pEntity;
}

//  *******************************************************************************************************************
void cEntityManager::VDeleteEntity(shared_ptr<IBaseEntity> const pNewEntity)
{
	shared_ptr<cBaseEntity> pEntity = dynamic_pointer_cast<cBaseEntity>(pNewEntity);
	if (pEntity != NULL)
	{
		auto Entityiter = m_EntityMap.find(pEntity->VGetEntityID());
		if (Entityiter == m_EntityMap.end())
		{
			return;
		}
		IBaseEntity::ComponentList components;

		pEntity->GetAllComponents(components);

		for (auto iter = components.begin(); iter != components.end(); iter++)
		{
			VRemoveComponent(pNewEntity, (*iter)->VGetID());
		}
		m_DeletedEntities.push_back(pEntity);
		m_EntityMap.erase(pEntity->VGetEntityID());
	}
}

//  *******************************************************************************************************************
int cEntityManager::VGetEntityID(const IBaseEntity * const pEntity) const
{
	if (pEntity != NULL)
	{
		pEntity->VGetEntityID();
	}
	return 0;
}

//  *******************************************************************************************************************
shared_ptr<IBaseEntity> const cEntityManager::VGetEntityFromID(const int iID) const
{
	auto iter = m_EntityMap.find(iID);
	if (iter == m_EntityMap.end())
	{
		return NULL;
	}
	return iter->second;
}

//  *******************************************************************************************************************
cString cEntityManager::VGetEntityNameFromID(const int iID) const
{
	shared_ptr<IBaseEntity> pEntity = VGetEntityFromID(iID);
	return VGetEntityName(pEntity);
}

//  *******************************************************************************************************************
cString cEntityManager::VGetEntityName(shared_ptr<IBaseEntity> const pEntity) const
{
	if (pEntity != NULL)
	{
		return pEntity->VGetEntityType().GetString();
	}
	return "";
}

//  *******************************************************************************************************************
shared_ptr<IBaseEntity> const cEntityManager::VGetEntityOfType(const Base::cHashedString & Type)
{
	for(auto it = m_EntityMap.begin(); it != m_EntityMap.end(); ++it)
	{
		shared_ptr<IBaseEntity> const pEntity = it->second;
		if (pEntity->VGetEntityType() == Type)
		{
			return pEntity;
		}
	}
	return NULL;
}

//  *******************************************************************************************************************
shared_ptr<IBaseComponent> cEntityManager::VGetComponent(shared_ptr<IBaseEntity> const pEntity, const cHashedString & ComponentName) const
{
	shared_ptr<cBaseEntity> const pEnt = dynamic_pointer_cast<cBaseEntity>(pEntity);
	if (pEnt != NULL)
	{
		return pEnt->GetComponent(ComponentName);
	}
	return NULL;
}

//  *******************************************************************************************************************
void cEntityManager::VAddComponent(shared_ptr<IBaseEntity> const pEntity, shared_ptr<IBaseComponent> pComponent)
{
	shared_ptr<cBaseEntity> pEnt = dynamic_pointer_cast<cBaseEntity>(pEntity);
	if (pEnt != NULL)
	{
		auto iter = m_EntityMap.find(pEnt->VGetEntityID());
		SP_ASSERT_DEBUG(iter != m_EntityMap.end())(pEntity->VGetEntityType().GetString()).SetCustomMessage("Adding component to unregistered entity");

		if (iter == m_EntityMap.end())
		{
			return;
		}

		if (pComponent != NULL)
		{
			pEnt->AddComponent(pComponent);
			auto iter = m_ComponentMap.find(pComponent->VGetID());
			if (iter == m_ComponentMap.end())
			{
				EntityList list;
				list.push_back(pEntity);
				m_ComponentMap.insert(std::make_pair(pComponent->VGetID(), list));
			}
			else
			{
				EntityList & list = iter->second;
				list.push_back(pEntity);
			}
		}
	}
}

//  *******************************************************************************************************************
void cEntityManager::VRemoveComponent(shared_ptr<IBaseEntity> const pEntity, const UINT64 ComponentID)
{
	shared_ptr<cBaseEntity> pEnt = dynamic_pointer_cast<cBaseEntity>(pEntity);
	if (pEnt != NULL)
	{
		auto Entityiter = m_EntityMap.find(pEnt->VGetEntityID());
		SP_ASSERT_DEBUG(Entityiter != m_EntityMap.end())(pEntity->VGetEntityType().GetString()).SetCustomMessage("Removing component from unregistered entity");
		if (Entityiter == m_EntityMap.end())
		{
			return;
		}

		pEnt->RemoveComponent(ComponentID);

		auto iter = m_ComponentMap.find(ComponentID);
		SP_ASSERT_DEBUG(iter != m_ComponentMap.end())(pEntity->VGetEntityType().GetString()).SetCustomMessage("Trying to remove non existent component");
		if (iter == m_ComponentMap.end())
		{
			return;
		}

		EntityList & list = iter->second;
		list.remove(pEntity);
		if (list.empty())
		{
			m_ComponentMap.erase(ComponentID);
		}
	}
}
//  *******************************************************************************************************************
shared_ptr<IBaseEntity> const cEntityManager::VGetFirstEntityWithComponent(const cHashedString & Component)
{
	auto iter = m_ComponentMap.find(Component.GetHash());
	if (iter != m_ComponentMap.end())
	{
		EntityList & entities = iter->second;
		return entities.front();
	}
	return NULL;
}

//  *******************************************************************************************************************
void cEntityManager::VGetEntitiesWithComponent(const cHashedString & Component, EntityList & entities)
{
	auto iter = m_ComponentMap.find(Component.GetHash());
	if (iter != m_ComponentMap.end())
	{
		entities = iter->second;
	}
}

//  *******************************************************************************************************************
void cEntityManager::VUpdate(const float deltaTime)
{
	auto iter = m_EntityMap.begin();
	for(iter != m_EntityMap.begin(); iter != m_EntityMap.end(); iter++)
	{
		shared_ptr<IBaseEntity> pEntity = iter->second;
		if (pEntity->VIsActive())
		{
			pEntity->VOnUpdate(deltaTime);
		}
	}
	RemoveDeletedEntities();
}

//  *******************************************************************************************************************
void cEntityManager::VDeleteAllEntities()
{
	for(auto iter = m_ComponentMap.begin(); iter != m_ComponentMap.end(); iter++)
	{
		iter->second.clear();
	}
	m_ComponentMap.clear();

	auto iter = m_EntityMap.begin();
	while(iter != m_EntityMap.end())
	{
		shared_ptr<IBaseEntity> pEntity = iter->second;
		iter++;
		pEntity.reset();
	}

	m_EntityMap.clear();
	m_DeletedEntities.clear();
}

//  *******************************************************************************************************************
void cEntityManager::InitializeEntity(shared_ptr<IBaseEntity> const pEntity)
{
	shared_ptr<cBaseEntity> pBaseEntity = dynamic_pointer_cast<cBaseEntity>(pEntity);
	m_EntityMap.insert(std::make_pair(pBaseEntity->VGetEntityID(), pEntity));

	SP_LOG(2, "Registering Entity")(pBaseEntity->VGetEntityID())(pEntity->VGetEntityType().GetString());

	pEntity->VInitialize();

	IBaseEntity::ComponentList components;
	pBaseEntity->GetAllComponents(components);

	for (auto iter = components.begin(); iter != components.end(); iter++)
	{
		VAddComponent(pEntity, *iter);
	}
}

//  *******************************************************************************************************************
void cEntityManager::Cleanup()
{
	VDeleteAllEntities();
}

//  *******************************************************************************************************************
void cEntityManager::RemoveDeletedEntities()
{
	for(auto iter = m_DeletedEntities.begin(); iter != m_DeletedEntities.end(); iter++)
	{
		shared_ptr<IBaseEntity> pEntity = (*iter);
		pEntity.reset();
	}
	m_DeletedEntities.clear();
}

//  *******************************************************************************************************************
void IEntityManager::CreateAsService()
{
	cServiceLocator::GetInstance()->Register<IEntityManager>(shared_ptr<IEntityManager>(DEBUG_NEW cEntityManager()));
}
