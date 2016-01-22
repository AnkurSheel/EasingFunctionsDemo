#include "stdafx.h"
#include "BTSequenceNode.h"

using namespace AI;
using namespace Base;

//  ********************************************************************************************************************
cBTSequenceNode::cBTSequenceNode()
	: m_CurrentChildIndex(-1)
{
}

//  ********************************************************************************************************************
cBTSequenceNode::~cBTSequenceNode()
{
}

//  ********************************************************************************************************************
void cBTSequenceNode::VOnInitialize(void * pOwner)
{
	cBTCompositeNode::VOnInitialize(pOwner);
	m_CurrentChildIndex = 0;
	BTNodeStrongPtr pChild = MakeStrongPtr(GetChildAt(m_CurrentChildIndex));
	if (pChild != NULL)
	{
		pChild->VOnInitialize(pOwner);
	}
}

//  ********************************************************************************************************************
BT_STATUS::Enum cBTSequenceNode::VOnUpdate(void * pOwner, float deltaTime)
{
	BT_STATUS::Enum result = BT_STATUS::Invalid;
	SP_ASSERT(m_CurrentChildIndex >= 0)(m_CurrentChildIndex >= m_Children.size()).SetCustomMessage("Trying to execute without calling Initialize");
	SP_ASSERT(m_Children.size() > 0).SetCustomMessage("Sequence should have atleast 1 child");
	if (m_Children.size() == 0 || m_CurrentChildIndex >= m_Children.size())
	{
		return result;
	}

	BTNodeStrongPtr pChild = MakeStrongPtr(GetChildAt(m_CurrentChildIndex));
	SP_ASSERT(pChild != NULL)(m_CurrentChildIndex).SetCustomMessage("Something went wrong, Child was NULL");
	if (pChild != NULL)
	{
		result = pChild->Tick(pOwner, deltaTime);
		if (result == BT_STATUS::Success)
		{
			++m_CurrentChildIndex;
			if (m_CurrentChildIndex >= m_Children.size())
			{
				return BT_STATUS::Success;
			}
			else
			{
				return BT_STATUS::Running;
			}
		}
	}
	return result;
}
