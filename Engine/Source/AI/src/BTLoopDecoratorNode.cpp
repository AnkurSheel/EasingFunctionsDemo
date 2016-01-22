#include "stdafx.h"
#include "BTLoopDecoratorNode.h"

using namespace AI;

//  ********************************************************************************************************************
cBTLoopDecoratorNode::cBTLoopDecoratorNode()
	: m_Count(0)
{
}

//  ********************************************************************************************************************
cBTLoopDecoratorNode::~cBTLoopDecoratorNode()
{
}

//  ********************************************************************************************************************
void cBTLoopDecoratorNode::VOnInitialize(void * pOwner)
{
	cBTDecoratorNode::VOnInitialize(pOwner);
	if (!m_Initialized)
	{
		m_Count = 0;
	}
}

//  ********************************************************************************************************************
BT_STATUS::Enum cBTLoopDecoratorNode::VOnUpdate(void * pOwner, float deltaTime)
{
	SP_ASSERT(m_Initialized).SetCustomMessage("Trying to execute without calling Initialize");
	SP_ASSERT(m_pChild != NULL).SetCustomMessage("Decorator should have 1 child");

	if (!m_Initialized || m_pChild == NULL || m_TerminateStatus != BT_STATUS::Invalid
		|| (m_Repeats.IsValid() && m_Count >= m_Repeats.GetValue()))
	{
		m_ReturnStatus = BT_STATUS::Invalid;
		return m_ReturnStatus;
	}

	m_ReturnStatus = m_pChild->Tick(pOwner, deltaTime);
	if (m_ReturnStatus == BT_STATUS::Success)
	{
		m_Count++;
		if (m_Repeats.IsValid() && m_Count >= m_Repeats.GetValue())
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
void cBTLoopDecoratorNode::VReset()
{
	if (m_ReturnStatus != BT_STATUS::Running)
	{
		m_Count = 0;
	}
	cBTDecoratorNode::VReset();
}
