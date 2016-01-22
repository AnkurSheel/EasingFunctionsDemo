#include "stdafx.h"
#include "AISystem.h"
#include "EntityManager.hxx"
#include "BaseEntity.hxx"
#include "AIComponent.h"

using namespace GameBase;
using namespace Utilities;
using namespace Base;
using namespace AI;

const cHashedString cAISystem::m_Type = cHashedString("aisystem");

//  *******************************************************************************************************************
cAISystem::cAISystem()
{
	VInitialize();
}

//  *******************************************************************************************************************
cAISystem::~cAISystem()
{
}

//  *******************************************************************************************************************
void cAISystem::VInitialize()
{
	cProcess::VInitialize();
	m_pEntityManager = cServiceLocator::GetInstance()->GetService<IEntityManager>();
}

//  *******************************************************************************************************************
void cAISystem::VUpdate(const float deltaTime)
{
	cProcess::VUpdate(deltaTime);

	shared_ptr<IEntityManager> pEntityManager = MakeStrongPtr(m_pEntityManager);
	if (pEntityManager == NULL)
	{
		return;
	}

	IEntityManager::EntityList entityList;
	pEntityManager->VGetEntitiesWithComponent(cAIComponent::GetName(), entityList);
	for(auto enityIter = entityList.begin(); enityIter != entityList.end(); enityIter++)
	{
		shared_ptr<IBaseEntity> pEntity = *enityIter;
		if (pEntity->VIsActive())
		{
			shared_ptr<cAIComponent> pAIComponent = CastToAIComponent(pEntity);
			if (pAIComponent != NULL)
			{
				pAIComponent->Update(deltaTime);
			}
		}
	}
}

//  *******************************************************************************************************************
shared_ptr<cAIComponent> const cAISystem::CastToAIComponent(shared_ptr<IBaseEntity> const pEntity)
{
	shared_ptr<IEntityManager> pEntityManager = MakeStrongPtr(m_pEntityManager);

	if (pEntityManager == NULL)
	{
		return NULL;
	}

	return (dynamic_pointer_cast<cAIComponent>(pEntityManager->VGetComponent(pEntity, cAIComponent::GetName())));
}
