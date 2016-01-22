#include "stdafx.h"
#include "BTBehaviorNode.h"

using namespace AI;
using namespace Base;

//  ********************************************************************************************************************
cBTBehaviorNode::cBTBehaviorNode()
	: m_ReturnStatus(BT_STATUS::Invalid)
	, m_TerminateStatus(BT_STATUS::Invalid)
	, m_Initialized(false)
{
}

//  ********************************************************************************************************************
cBTBehaviorNode::~cBTBehaviorNode()
{
}

//  ********************************************************************************************************************
void cBTBehaviorNode::VOnInitialize(void * pOwner)
{
	VReset();
	m_Initialized = true;
	m_ReturnStatus = BT_STATUS::Running;
}

//  *******************************************************************************************************************
void cBTBehaviorNode::VReset()
{
	m_Initialized = false;
	m_TerminateStatus = BT_STATUS::Invalid;
}

//  ********************************************************************************************************************
void cBTBehaviorNode::VOnTerminate(const BT_STATUS::Enum status)
{
	if (status != BT_STATUS::Invalid)
	{
		m_TerminateStatus = status;
	}
	m_Initialized = false;
}

//  ********************************************************************************************************************
BT_STATUS::Enum cBTBehaviorNode::Tick(void * pOwner, float deltaTime)
{
	if (!m_Initialized)
	{
		VOnInitialize(pOwner);
	}

	m_ReturnStatus = VOnUpdate(pOwner, deltaTime);
	if (m_ReturnStatus != BT_STATUS::Running)
	{
		VOnTerminate(m_ReturnStatus);
	}
	return m_ReturnStatus;
}

//  *******************************************************************************************************************
void cBTBehaviorNode::Abort()
{
	VOnTerminate(BT_STATUS::Aborted);
	m_ReturnStatus = BT_STATUS::Aborted;
}
