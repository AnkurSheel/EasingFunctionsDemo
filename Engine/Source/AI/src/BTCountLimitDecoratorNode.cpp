#include "stdafx.h"
#include "BTCountLimitDecoratorNode.h"

using namespace AI;

//  ********************************************************************************************************************
cBTCountLimitDecoratorNode::cBTCountLimitDecoratorNode()
: m_Count(0)
, m_Limit(0)
, m_AllowReinitialize(false)
{
}

//  ********************************************************************************************************************
cBTCountLimitDecoratorNode::~cBTCountLimitDecoratorNode()
{
}

//  ********************************************************************************************************************
void cBTCountLimitDecoratorNode::VOnInitialize(void * pOwner)
{
	cBTDecoratorNode::VOnInitialize(pOwner);
	if (m_AllowReinitialize && m_Count == m_Limit)
	{
		VReset();
	}
}

//  ********************************************************************************************************************
BT_STATUS::Enum cBTCountLimitDecoratorNode::VOnUpdate(void * pOwner, float deltaTime)
{
	m_ReturnStatus = BT_STATUS::Invalid;
	SP_ASSERT(m_Initialized).SetCustomMessage("Trying to execute without calling Initialize");
	SP_ASSERT(m_pChild != NULL).SetCustomMessage("Decorator should have 1 child");

	if (!m_Initialized || m_pChild == NULL || m_TerminateStatus != BT_STATUS::Invalid || (m_Limit != -1 && m_Count >= m_Limit))
	{
		if (m_AllowReinitialize)
		{
			VOnInitialize(pOwner);
		}
		else
		{
			return m_ReturnStatus;
		}
	}

	m_ReturnStatus = m_pChild->Tick(pOwner, deltaTime);
	if (m_ReturnStatus == BT_STATUS::Success)
	{
		m_Count++;
		if (m_Limit != -1 && m_Count >= m_Limit)
		{
			m_ReturnStatus = BT_STATUS::Success;
		}
		else
		{
			m_ReturnStatus = BT_STATUS::Running;
			VOnInitialize(pOwner);
		}
	}
	return m_ReturnStatus;
}

//  *******************************************************************************************************************
void cBTCountLimitDecoratorNode::VReset()
{
	if (m_ReturnStatus != BT_STATUS::Running)
	{
		m_Count = 0;
	}
	cBTDecoratorNode::VReset();
}
