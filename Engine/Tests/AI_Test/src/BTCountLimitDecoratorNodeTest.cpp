#include "stdafx.h"
#include "BTCountLimitDecoratorNode.h"
#include "BTNodeFactory.h"
#include "MockBTNode.h"
#include "BtBehaviorTree.h"

using namespace AI;
using namespace Base;
using namespace testing;

class BTCountLimitDecoratorNodeTest : public ::testing::Test
{
protected:
	static void SetUpTestCase()
	{
		m_pOutput = new AutoCaptureOutput();
	}

	static void TearDownTestCase() 
	{
		SafeDelete(&m_pOutput);
	}

	void SetUp()
	{

	}

	void TearDown()
	{
	}

	static AutoCaptureOutput * m_pOutput;
};

AutoCaptureOutput * BTCountLimitDecoratorNodeTest::m_pOutput;

//  *******************************************************************************************************************
TEST_F(BTCountLimitDecoratorNodeTest, ZeroLimit)
{
	shared_ptr<cMockBTNode> pSuccessNode(new cMockBTNode);
	EXPECT_CALL(*pSuccessNode, VOnUpdate(NULL, 0.0f))
		.WillRepeatedly(Return(BT_STATUS::Success));

	shared_ptr<cBTCountLimitDecoratorNode> pCountLimitNode = cBTNodeFactory::Create<cBTCountLimitDecoratorNode>()
		->SetChild<cBTCountLimitDecoratorNode>(pSuccessNode);

	BTTreeStrongPtr pBT(DEBUG_NEW cBTBehaviorTree(pCountLimitNode));

	BT_STATUS::Enum result = pBT->Tick(NULL, 0.0f);
	EXPECT_EQ(BT_STATUS::Invalid, result);
}

//  *******************************************************************************************************************
TEST_F(BTCountLimitDecoratorNodeTest, ThreeLimit)
{
	shared_ptr<cMockBTNode> pSuccessNode(new cMockBTNode);
	EXPECT_CALL(*pSuccessNode, VOnUpdate(NULL, 0.0f))
		.WillRepeatedly(Return(BT_STATUS::Success));

	shared_ptr<cBTCountLimitDecoratorNode> pCountLimitNode = cBTNodeFactory::Create<cBTCountLimitDecoratorNode>()
		->SetLimit(3)
		->SetChild<cBTCountLimitDecoratorNode>(pSuccessNode);

	BTTreeStrongPtr pBT(DEBUG_NEW cBTBehaviorTree(pCountLimitNode));

	BT_STATUS::Enum result = pBT->Tick(NULL, 0.0f);
	EXPECT_EQ(BT_STATUS::Running, result);
	result = pBT->Tick(NULL, 0.0f);
	EXPECT_EQ(BT_STATUS::Running, result);
	result = pBT->Tick(NULL, 0.0f);
	EXPECT_EQ(BT_STATUS::Success, result);
}

//  *******************************************************************************************************************
TEST_F(BTCountLimitDecoratorNodeTest, TwoLimitRunningSuccess)
{
	shared_ptr<cMockBTNode> pSuccessNode(new cMockBTNode);
	EXPECT_CALL(*pSuccessNode, VOnUpdate(NULL, 0.0f))
		.Times(AnyNumber())
		.WillOnce(Return(BT_STATUS::Running))
		.WillOnce(Return(BT_STATUS::Success))
		.WillOnce(Return(BT_STATUS::Running))
		.WillOnce(Return(BT_STATUS::Success))
		.WillOnce(Return(BT_STATUS::Running))
		.WillOnce(Return(BT_STATUS::Success))
		.WillOnce(Return(BT_STATUS::Running))
		.WillOnce(Return(BT_STATUS::Success));

	shared_ptr<cBTBehaviorNode> pCountLimitNode = cBTNodeFactory::Create<cBTCountLimitDecoratorNode>()
		->SetLimit(2)
		->SetAllowReinitialize(true)
		->SetChild<cBTCountLimitDecoratorNode>(pSuccessNode);

	BTTreeStrongPtr pBT(DEBUG_NEW cBTBehaviorTree(pCountLimitNode));

	for (int i=0;i<2; i++)
	{
		BT_STATUS::Enum result = pBT->Tick(NULL, 0.0f);
		EXPECT_EQ(BT_STATUS::Running, result);
		result = pBT->Tick(NULL, 0.0f);
		EXPECT_EQ(BT_STATUS::Running, result);
		result = pBT->Tick(NULL, 0.0f);
		EXPECT_EQ(BT_STATUS::Running, result);
		result = pBT->Tick(NULL, 0.0f);
		EXPECT_EQ(BT_STATUS::Success, result);
		pCountLimitNode->VReset();
	}
}

//  *******************************************************************************************************************
TEST_F(BTCountLimitDecoratorNodeTest, TwoLimitRunningFailure)
{
	shared_ptr<cMockBTNode> pFailureNode(new cMockBTNode);
	EXPECT_CALL(*pFailureNode, VOnUpdate(NULL, 0.0f))
		.Times(AnyNumber())
		.WillOnce(Return(BT_STATUS::Running))
		.WillOnce(Return(BT_STATUS::Failure))
		.WillOnce(Return(BT_STATUS::Running))
		.WillOnce(Return(BT_STATUS::Failure))
		.WillOnce(Return(BT_STATUS::Running))
		.WillOnce(Return(BT_STATUS::Failure))
		.WillOnce(Return(BT_STATUS::Running))
		.WillOnce(Return(BT_STATUS::Failure));

	shared_ptr<cBTBehaviorNode> pCountLimitNode = cBTNodeFactory::Create<cBTCountLimitDecoratorNode>()
		->SetLimit(2)
		->SetAllowReinitialize(true)
		->SetChild<cBTCountLimitDecoratorNode>(pFailureNode);

	BTTreeStrongPtr pBT(DEBUG_NEW cBTBehaviorTree(pCountLimitNode));

	for (int i=0;i<2; i++)
	{
		BT_STATUS::Enum result = pBT->Tick(NULL, 0.0f);
		EXPECT_EQ(BT_STATUS::Running, result);
		result = pBT->Tick(NULL, 0.0f);
		EXPECT_EQ(BT_STATUS::Failure, result);
		result = pBT->Tick(NULL, 0.0f);
		EXPECT_EQ(BT_STATUS::Running, result);
		result = pBT->Tick(NULL, 0.0f);
		EXPECT_EQ(BT_STATUS::Failure, result);
		pCountLimitNode->VReset();
	}
}

//  *******************************************************************************************************************
TEST_F(BTCountLimitDecoratorNodeTest, TwoLimitDisableReinit)
{
	shared_ptr<cMockBTNode> pSuccessNode(new cMockBTNode);
	EXPECT_CALL(*pSuccessNode, VOnUpdate(NULL, 0.0f))
		.Times(AnyNumber())
		.WillRepeatedly(Return(BT_STATUS::Success));

	shared_ptr<cBTCountLimitDecoratorNode> pCountLimitNode = cBTNodeFactory::Create<cBTCountLimitDecoratorNode>()
		->SetLimit(2)
		->SetAllowReinitialize(false)
		->SetChild<cBTCountLimitDecoratorNode>(pSuccessNode);

	BTTreeStrongPtr pBT(DEBUG_NEW cBTBehaviorTree(pCountLimitNode));

	BT_STATUS::Enum result = pBT->Tick(NULL, 0.0f);
	EXPECT_EQ(BT_STATUS::Running, result);
	result = pBT->Tick(NULL, 0.0f);
	EXPECT_EQ(BT_STATUS::Success, result);

	for (int i=0; i < 2; i++)
	{
		result = pBT->Tick(NULL, 0.0f);
		EXPECT_EQ(BT_STATUS::Running, result);
		result = pBT->Tick(NULL, 0.0f);
		EXPECT_EQ(BT_STATUS::Success, result);
	}
}
