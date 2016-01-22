#include "stdafx.h"
#include "BTDecoratorNode.h"

using namespace AI;
using namespace Base;
//  ********************************************************************************************************************
cBTDecoratorNode::cBTDecoratorNode()
	: m_pChild(NULL)
{
}

//  ********************************************************************************************************************
cBTDecoratorNode::~cBTDecoratorNode()
{
}

//  ********************************************************************************************************************
void cBTDecoratorNode::VOnInitialize(void * pOwner)
{
	cBTBehaviorNode::VOnInitialize(pOwner);
	m_pChild->VOnInitialize(pOwner);
}

//  ********************************************************************************************************************
BT_STATUS::Enum cBTDecoratorNode::VOnUpdate(void * pOwner, float deltaTime)
{
	BT_STATUS::Enum result = BT_STATUS::Invalid;
	SP_ASSERT(m_Initialized).SetCustomMessage("Trying to execute without calling Initialize");
	SP_ASSERT(m_pChild != NULL).SetCustomMessage("Decorator should have 1 child");

	if (!m_Initialized || m_pChild == NULL)
	{
		return result;
	}

	if (m_pChild != NULL)
	{
		result = m_pChild->Tick(pOwner, deltaTime);
	}
	return result;
}

//  *******************************************************************************************************************
void cBTDecoratorNode::VReset()
{
	cBTBehaviorNode::VReset();
	if (m_pChild != NULL)
	{
		m_pChild->VReset();
	}
}
