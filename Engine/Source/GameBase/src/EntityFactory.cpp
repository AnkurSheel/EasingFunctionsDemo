#include "StdAfx.h"
#include "EntityFactory.h"
#include "BaseEntity.h"
#include "Config.h"
#include "ComponentFactory.h"

using namespace GameBase;
using namespace Utilities;
using namespace Base;

cEntityFactory * cEntityFactory::m_pInstance = NULL;

//  *******************************************************************************************************************
cEntityFactory::cEntityFactory()
{
	if (m_pInstance == NULL)
	m_pInstance = this;
}

//  *******************************************************************************************************************
cEntityFactory::~cEntityFactory()
{
}

//  ********************************************************************************************************************
shared_ptr<IBaseEntity> cEntityFactory::VCreateEntity(const cHashedString & Type)
{
	shared_ptr<cBaseEntity> pEntity = dynamic_pointer_cast<cBaseEntity>(m_RegisteredEntities.Create(Type.GetHash()));
	SP_ASSERT_ERROR(pEntity != NULL)(Type.GetString()).SetCustomMessage("Entity not Registered");
	if (pEntity != NULL)
	{
		const cEntityDef * const pEntityDef = cConfig::GetEntityDef(Type);
		if (pEntityDef != NULL)
		{
			for (auto Iter = pEntityDef->m_Components.begin(); Iter != pEntityDef->m_Components.end(); Iter++)
			{
				shared_ptr<IBaseComponent> pComponent = cComponentFactory::Instance()->GetDuplicateComponent(Iter->second);
				if (pComponent != NULL)
				{
					pEntity->AddComponent(pComponent);
				}
			}
		}
	}

	return pEntity;
}
