#include "stdafx.h"
#include "MockBTNode.h"

using namespace testing;

//  *******************************************************************************************************************
cMockBTNode::cMockBTNode() 
	: m_InitializeCalled(0)
	, m_UpdateCalled(0)
	, m_TerminateCalled(0)
{

}
//  *******************************************************************************************************************
void cMockBTNode::DelegateToFake()
{
	ON_CALL(*this, VOnInitialize(_))
		.WillByDefault(Invoke(this, &cMockBTNode::Enter));
	ON_CALL(*this, VOnUpdate(_, _))
		.WillByDefault(Invoke(this, &cMockBTNode::Execute));
	ON_CALL(*this, VOnTerminate(_))
		.WillByDefault(Invoke(this, &cMockBTNode::Exit));
}

//  *******************************************************************************************************************
void cMockBTNode::Enter(void * pOwner)
{
	++m_InitializeCalled;
	cBTBehaviorNode::VOnInitialize(pOwner);
}

//  *******************************************************************************************************************
BT_STATUS::Enum cMockBTNode::Execute(void * pOwner, float deltaTime)
{
	++m_UpdateCalled;
	return m_ReturnStatus;
}
//  *******************************************************************************************************************
void cMockBTNode::Exit(const BT_STATUS::Enum status)
{
	++m_TerminateCalled;
	m_TerminateStatus = status;
}
