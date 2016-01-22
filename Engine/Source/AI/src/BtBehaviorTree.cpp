#include "stdafx.h"
#include "BtBehaviorTree.h"
#include "BTBehaviorNode.h"

using namespace AI;

//  *******************************************************************************************************************
cBTBehaviorTree::cBTBehaviorTree(BTNodeStrongPtr pRoot)
	: m_pRoot(pRoot)
{
}

//  *******************************************************************************************************************
cBTBehaviorTree::~cBTBehaviorTree()
{
	m_pRoot.reset();
}

//  *******************************************************************************************************************
BT_STATUS::Enum  cBTBehaviorTree::Tick(void * pOwner, float deltaTime)
{
	SP_ASSERT(m_pRoot != NULL).SetCustomMessage("Behavior Tree root is Null");
	if (m_pRoot != NULL)
	{
		return m_pRoot->Tick(pOwner, deltaTime);
	}
	return BT_STATUS::Invalid;
}
