#include "stdafx.h"
#include "BTBaseSelectorNode.h"

using namespace AI;
using namespace Base;


//  *******************************************************************************************************************
cBTBaseSelectorNode::cBTBaseSelectorNode()
	: m_CurrentChildIndex(-1)
{
}

//  ********************************************************************************************************************
cBTBaseSelectorNode::~cBTBaseSelectorNode()
{
}

//  ********************************************************************************************************************
void cBTBaseSelectorNode::VOnInitialize(void * pOwner)
{
	cBTCompositeNode::VOnInitialize(pOwner);
	m_CurrentChildIndex = -1;
	VCalculateNextChildIndex();
	BTNodeStrongPtr pChild = MakeStrongPtr(GetChildAt(m_CurrentChildIndex));
	if (pChild != NULL)
	{
		pChild->VOnInitialize(pOwner);
	}
}

//  ********************************************************************************************************************
BT_STATUS::Enum cBTBaseSelectorNode::VOnUpdate(void * pOwner, float deltaTime)
{
	SP_ASSERT(m_CurrentChildIndex >= 0 && m_CurrentChildIndex < m_Children.size())(m_CurrentChildIndex).SetCustomMessage("Trying to execute without calling Initialize");
	SP_ASSERT(m_Children.size() > 0).SetCustomMessage("Priority Node should have atleast 1 child");
	if (m_Children.size() == 0 || m_CurrentChildIndex >= m_Children.size())
	{
		return BT_STATUS::Invalid;
	}

	BTNodeStrongPtr pChild = MakeStrongPtr(GetChildAt(m_CurrentChildIndex));
	SP_ASSERT(pChild != NULL)(m_CurrentChildIndex).SetCustomMessage("Something went wrong, Child was NULL");
	if (pChild != NULL)
	{
		m_ReturnStatus = pChild->Tick(pOwner, deltaTime);
		if (m_ReturnStatus == BT_STATUS::Failure)
		{
			VCalculateNextChildIndex();
			if (VIsDone())
			{
				VOnInitialize(pOwner);
				return BT_STATUS::Failure;
			}
			else
			{
				m_ReturnStatus = BT_STATUS::Running;
			}
		}
		else if (m_ReturnStatus == BT_STATUS::Success)
		{
			VOnInitialize(pOwner);
			return BT_STATUS::Success;
		}
	}
	return m_ReturnStatus;
}
