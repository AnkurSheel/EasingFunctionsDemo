//  *******************************************************************************************************************
//  BTMockErrorNode   version:  1.0   Ankur Sheel  date: 2014/12/17
//  *******************************************************************************************************************
// 
//  *******************************************************************************************************************
#ifndef __BTMOCKERRORNODE_H__
#define __BTMOCKERRORNODE_H__

#include "BTBehaviorNode.h"
#include "gmock/gmock.h"

using namespace AI;

class cMockBTNode 
: public cBTBehaviorNode
{
public:
	MOCK_METHOD1(VOnInitialize, void(void * pOwner));
	MOCK_METHOD2(VOnUpdate, BT_STATUS::Enum(void * pOwner, float deltaTime));
	MOCK_METHOD1(VOnTerminate, void(const BT_STATUS::Enum));

	cMockBTNode();

	void Enter(void * pOwner);
	void SetStatus(BT_STATUS::Enum status) { m_ReturnStatus = status; }
	BT_STATUS::Enum Execute(void * pOwner, float deltaTime);
	void Exit(const BT_STATUS::Enum status);

	void DelegateToFake();

public:
	int m_InitializeCalled;
	int m_UpdateCalled;
	int m_TerminateCalled;
};

#endif  // __BTMOCKERRORNODE_H__
