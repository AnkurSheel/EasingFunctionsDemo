//  *******************************************************************************************************************
//  TransformComponent version:  1.0   Ankur Sheel  date: 2013/04/10
//  *******************************************************************************************************************
//  purpose:
//  *******************************************************************************************************************
#include "stdafx.h"
#include "TransformComponent.h"
#include "XMLNode.hxx"
#include "EventManager.hxx"
#include "BaseEntity.hxx"
#include "EntityMovedEventData.h"
#include "EntityScaledEventData.h"
#include "EntityRotatedEventData.h"

using namespace GameBase;
using namespace Base;
using namespace Utilities;

cHashedString	cTransformComponent::m_Name = cHashedString("transformcomponent");
cVector3 cTransformComponent::m_Forward(0.0f, -1.0f, 0.0f);

//  *******************************************************************************************************************
cTransformComponent::cTransformComponent()
{
}

//  *******************************************************************************************************************
cTransformComponent::~cTransformComponent()
{
}

//  *******************************************************************************************************************
void cTransformComponent::VInitialize(const IXMLNode * const pXMLNode)
{
	if (pXMLNode == NULL)
	{
		return;
	}

	m_Position = pXMLNode->VGetChildValueAsVector3("Position", cVector3::Zero);
	m_Size = pXMLNode->VGetChildValueAsVector3("ScaleInPixels", cVector3(8.0f, 8.0f, 8.0f));
	m_Rotation = pXMLNode->VGetChildValueAsVector3("Orientation", cVector3::Zero);

	ReCalculateLookAt();
}

//  *******************************************************************************************************************
shared_ptr<IXMLNode> cTransformComponent::VGenerateXml() const
{
	shared_ptr<IXMLNode> pXmlNode = IXMLNode::Create("TransformComponent");

	shared_ptr<IXMLNode> pChildNode = MakeStrongPtr(pXmlNode->VAddElement("Position", ""));
	pChildNode->VAddAttribute("x", static_cast<int>(m_Position.x));
	pChildNode->VAddAttribute("y", static_cast<int>(m_Position.y));
	pChildNode->VAddAttribute("z", static_cast<int>(m_Position.z));

	pChildNode = MakeStrongPtr(pXmlNode->VAddElement("Orientation", ""));
	pChildNode->VAddAttribute("x", static_cast<float>(RadtoDeg(m_Rotation.x)));
	pChildNode->VAddAttribute("y", static_cast<float>(RadtoDeg(m_Rotation.y)));
	pChildNode->VAddAttribute("z", static_cast<float>(RadtoDeg(m_Rotation.z)));

	pChildNode = MakeStrongPtr(pXmlNode->VAddElement("ScaleInPixels", ""));
	pChildNode->VAddAttribute("x", static_cast<int>(m_Size.x));
	pChildNode->VAddAttribute("y", static_cast<int>(m_Size.y));
	pChildNode->VAddAttribute("z", static_cast<int>(m_Size.z));

	return pXmlNode;
}

//  *******************************************************************************************************************
void cTransformComponent::VCleanup()
{
}

//  *******************************************************************************************************************
void cTransformComponent::SetPosition(const cVector3 & position)
{
	if (m_Position != position)
	{
		m_Position = position;
		shared_ptr<IEventManager> pEventManager = MakeStrongPtr(cServiceLocator::GetInstance()->GetService<IEventManager>());
		if (pEventManager != NULL && !m_pOwner.expired())
		{
			shared_ptr<cEntityMovedEventData> pEvent(DEBUG_NEW cEntityMovedEventData(m_Position, MakeStrongPtr(m_pOwner)->VGetEntityID()));
			pEventManager->VQueueEvent(pEvent);
		}
	}
}

//  *******************************************************************************************************************
void cTransformComponent::SetSize(const cVector3 & size)
{
	if (m_Size != size)
	{
		m_Size = size;
		shared_ptr<IEventManager> pEventManager = MakeStrongPtr(cServiceLocator::GetInstance()->GetService<IEventManager>());
		if (pEventManager != NULL && !m_pOwner.expired())
		{
			shared_ptr<cEntityScaledEventData> pEvent(DEBUG_NEW cEntityScaledEventData(m_Size, MakeStrongPtr(m_pOwner)->VGetEntityID()));
			pEventManager->VQueueEvent(pEvent);
		}
	}
}

//  *******************************************************************************************************************
void cTransformComponent::SetRotation(const cVector3 & rotation)
{
	if (m_Rotation != rotation)
	{
		m_Rotation = rotation;
		ReCalculateLookAt();

		shared_ptr<IEventManager> pEventManager = MakeStrongPtr(cServiceLocator::GetInstance()->GetService<IEventManager>());
		if (pEventManager != NULL && !m_pOwner.expired())
		{
			shared_ptr<cEntityRotatedEventData> pEvent(DEBUG_NEW cEntityRotatedEventData(m_Rotation, MakeStrongPtr(m_pOwner)->VGetEntityID()));
			pEventManager->VQueueEvent(pEvent);
		}
	}
}

//  *******************************************************************************************************************
void cTransformComponent::ReCalculateLookAt()
{
	m_LookAt = cVector3::RotateAboutZ(m_Forward, m_Rotation.z);
}
