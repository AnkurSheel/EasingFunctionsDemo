//  *******************************************************************************************************************
//  BaseEntity   version:  1.0   Ankur Sheel  date: 2012/07/30
//  *******************************************************************************************************************
//  purpose:
//  *******************************************************************************************************************
#include "stdafx.h"
#include "BaseEntity.h"
#include "StateMachine.h"
#include "BaseComponent.h"
#include "EntityManager.hxx"
#include "TransformComponent.h"
#include "EntityInitializedEventData.h"
#include "EventManager.hxx"
#include "XMLNode.hxx"

using namespace AI;
using namespace Utilities;
using namespace Base;
using namespace GameBase;

int cBaseEntity::m_sNextValidID = 1;

//  *******************************************************************************************************************
cEntityDef::cEntityDef()
{
}

//  *******************************************************************************************************************
cEntityDef::~cEntityDef()
{
	Cleanup();
}

//  *******************************************************************************************************************
void cEntityDef::Cleanup()
{
	auto iter = m_Components.begin();
	while(iter != m_Components.end())
	{
		shared_ptr<IBaseComponent> pComponent = iter->second;
		iter++;
		pComponent.reset();
	}
	m_Components.clear();
}

//  *******************************************************************************************************************
cBaseEntity::cBaseEntity()
	: m_Active(true)
{
	SetEntityID(m_sNextValidID);
}

//  *******************************************************************************************************************
cBaseEntity::cBaseEntity(const int ID)
{
	SetEntityID(ID);
}

//  *******************************************************************************************************************
cBaseEntity::~cBaseEntity()
{
	m_pTransFormComponent.reset();
}

//  *******************************************************************************************************************
void cBaseEntity::SetEntityID(const int ID)
{
	SP_ASSERT_DEBUG(ID >= m_sNextValidID).SetCustomMessage(cStringUtilities::MakeFormatted("%d is less than valid id. Setting ID to %d", m_ID, m_sNextValidID));
	if (ID >= m_sNextValidID)
	{
		m_ID = ID;
	}
	else
	{
		m_ID = m_sNextValidID;
	}
	m_sNextValidID = m_ID + 1;
}

//  *******************************************************************************************************************
void cBaseEntity::VInitialize()
{
	m_pTransFormComponent = dynamic_pointer_cast<cTransformComponent>(GetComponent(cTransformComponent::GetName().GetHash()));
}

//  *******************************************************************************************************************
void cBaseEntity::VOnInitialized(bool reInitialize)
{
	shared_ptr<IEventManager> pEventManager = MakeStrongPtr(cServiceLocator::GetInstance()->GetService<IEventManager>());
	if (pEventManager != NULL  && !m_pTransFormComponent.expired())
	{
		shared_ptr<cTransformComponent> pTransformComponentStrongPtr = MakeStrongPtr(m_pTransFormComponent);
		shared_ptr<cEntityInitializedEventData> pEvent(DEBUG_NEW cEntityInitializedEventData(pTransformComponentStrongPtr->GetPosition(),
			pTransformComponentStrongPtr->GetRotation(), pTransformComponentStrongPtr->GetSize(), reInitialize, VGetEntityID()));

		pEventManager->VTriggerEvent(pEvent);
	}
}

//  *******************************************************************************************************************
void cBaseEntity::VOnRestart()
{
	shared_ptr<IEventManager> pEventManager = MakeStrongPtr(cServiceLocator::GetInstance()->GetService<IEventManager>());
	if (pEventManager != NULL  && !m_pTransFormComponent.expired())
	{
		shared_ptr<cTransformComponent> pTransformComponentStrongPtr = MakeStrongPtr(m_pTransFormComponent);
		shared_ptr<cEntityInitializedEventData> pEvent(DEBUG_NEW cEntityInitializedEventData(pTransformComponentStrongPtr->GetPosition(),
			pTransformComponentStrongPtr->GetRotation(), pTransformComponentStrongPtr->GetSize(), true, VGetEntityID()));

		pEventManager->VTriggerEvent(pEvent);
	}
}

//  *******************************************************************************************************************
void cBaseEntity::VCleanup()
{
	ComponentMap::iterator iter;
	for(iter = m_Components.begin(); iter != m_Components.end(); ++iter)
	{
		iter->second.reset();
	}
	m_Components.clear();
	/*shared_ptr<IEntityManager> pEntityManager = (MakeStrongPtr<IEntityManager>(cServiceLocator::GetInstance()->GetService<IEntityManager>()));
	if (pEntityManager != NULL)
	{
		pEntityManager->VDeleteEntity(shared_from_this());
	}*/
}

//  *******************************************************************************************************************
bool cBaseEntity::VOnHandleMessage(const AI::Telegram & telegram)
{
	return false;
}

//  *******************************************************************************************************************
void cBaseEntity::AddComponent(shared_ptr<IBaseComponent> pComponent)
{
	auto iter = m_Components.find(pComponent->VGetID());
	if (iter == m_Components.end())
	{
		pComponent->VOnAttached(shared_from_this());
		m_Components.insert(std::make_pair(pComponent->VGetID(), pComponent));
	}
}

//  *******************************************************************************************************************
void cBaseEntity::RemoveComponent(const UINT64 ComponentID)
{
	shared_ptr<IBaseComponent> pComponent = GetComponent(ComponentID);
	if (pComponent != NULL)
	{
		m_Components.erase(ComponentID);
		pComponent.reset();
	}
}

//  *******************************************************************************************************************
shared_ptr<IBaseComponent> cBaseEntity::GetComponent(const cHashedString & ComponentName) const
{
	return GetComponent(ComponentName.GetHash());
}

//  *******************************************************************************************************************
shared_ptr<IBaseComponent> cBaseEntity::GetComponent(const UINT64 ComponentID) const
{
	auto iter = m_Components.find(ComponentID);
	if (iter == m_Components.end())
	{
		return NULL;
	}
	return iter->second;
}

//  *******************************************************************************************************************
void cBaseEntity::GetAllComponents(ComponentList & components) const
{
	components.clear();
	for(auto iter = m_Components.begin(); iter != m_Components.end(); iter++)
	{
		components.push_back(iter->second);
	}
}

//  *******************************************************************************************************************
void cBaseEntity::VHandleInput(const float inDeltaTime)
{
	SP_ASSERT_ERROR(false).SetCustomMessage("Should be handled by derived class");
}

//  *******************************************************************************************************************
cString cBaseEntity::VToXml()
{
	shared_ptr<IXMLNode> pXmlNode = IXMLNode::Create(VGetEntityType().GetString() + cStringUtilities::MakeFormatted("_id_%d", VGetEntityID()));
	pXmlNode->VAddAttribute("type", VGetEntityType().GetString());
	// components
	for (auto it = m_Components.begin(); it != m_Components.end(); ++it)
	{
		shared_ptr<IBaseComponent> pComponent = it->second;
		shared_ptr<IXMLNode> pComponentNode = pComponent->VGenerateXml();
		if (pComponentNode != NULL)
		{
			pXmlNode->VAddChild(pComponentNode);
		}
	}
	return pXmlNode->VToString();
}

//  *******************************************************************************************************************
void cBaseEntity::VSetActive(const bool active)
{
	m_Active = active;
	for(auto iter = m_Components.begin(); iter != m_Components.end(); iter++)
	{
		iter->second->VSetActive(active);
	}
}
