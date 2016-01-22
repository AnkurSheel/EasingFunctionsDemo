#include "stdafx.h"
#include "AIComponent.h"
#include "BaseEntity.hxx"
#include "XMLNode.hxx"
#include "BtBehaviorTree.h"

using namespace GameBase;
using namespace Base;
using namespace Utilities;
using namespace AI;

cHashedString cAIComponent::m_Name = cHashedString("aicomponent");

//  *******************************************************************************************************************
cAIComponent::cAIComponent()
{
}

//  *******************************************************************************************************************
cAIComponent::~cAIComponent()
{
}

//  *******************************************************************************************************************
void cAIComponent::VInitialize(const IXMLNode * const pXMLNode)
{
}

//  *******************************************************************************************************************
void cAIComponent::VCleanup()
{
	m_pBrain.reset();
}

//  *******************************************************************************************************************
void cAIComponent::Update(const float deltaTime)
{
	if (m_pBrain != NULL)
	{
		m_pBrain->Tick(MakeStrongPtr(m_pOwner).get(), deltaTime);
	}
}

//  *******************************************************************************************************************
shared_ptr<IXMLNode> cAIComponent::VGenerateXml() const
{
	shared_ptr<IXMLNode> pXmlNode = IXMLNode::Create("AIComponent");

	return pXmlNode;
}

