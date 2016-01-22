#include "stdafx.h"
#include "BTParallelNode.h"
#include "PrintTextNode.h"
#include "BTNodeFactory.h"
#include "MockBTNode.h"
#include "BtBehaviorTree.h"

using namespace AI;
using namespace Base;
using namespace ::testing;

class BTParallelTest : public ::testing::Test
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

AutoCaptureOutput * BTParallelTest::m_pOutput = NULL;

//  *******************************************************************************************************************
TEST_F(BTParallelTest, TestBTParallelErrors)
{
	shared_ptr<cBTParallelNode> pParallelNode = cBTNodeFactory::Create<cBTParallelNode>();
	BTTreeStrongPtr pBT(DEBUG_NEW cBTBehaviorTree(pParallelNode));

	BT_STATUS::Enum result = pBT->Tick(NULL, 0.0f);
	EXPECT_EQ(BT_STATUS::Invalid, result);

	result = pBT->Tick(NULL, 0.0f);
	EXPECT_EQ(BT_STATUS::Invalid, result);
}

//  *******************************************************************************************************************
TEST_F(BTParallelTest, OneChildFails)
{
	shared_ptr<cMockBTNode> pFailureNode(new cMockBTNode);
	EXPECT_CALL(*pFailureNode, VOnUpdate(NULL, 0.0f))
		.WillRepeatedly(Return(BT_STATUS::Failure));

	shared_ptr<cBTParallelNode> pParallelNode = cBTNodeFactory::Create<cBTParallelNode>()
		->AddChild<cBTParallelNode>(pFailureNode);

	BTTreeStrongPtr pBT(DEBUG_NEW cBTBehaviorTree(pParallelNode));

	BT_STATUS::Enum result = pBT->Tick(NULL, 0.0f);
	EXPECT_EQ(BT_STATUS::Failure, result);
}

//  *******************************************************************************************************************
TEST_F(BTParallelTest, OneChildCompletes)
{
	shared_ptr<cMockBTNode> pSuccessNode(new cMockBTNode);
	EXPECT_CALL(*pSuccessNode, VOnUpdate(NULL, 0.0f))
		.WillRepeatedly(Return(BT_STATUS::Success));

	shared_ptr<cBTParallelNode> pParallelNode = cBTNodeFactory::Create<cBTParallelNode>()
		->AddChild<cBTParallelNode>(pSuccessNode);

	BTTreeStrongPtr pBT(DEBUG_NEW cBTBehaviorTree(pParallelNode));

	BT_STATUS::Enum result = pBT->Tick(NULL, 0.0f);
	EXPECT_EQ(BT_STATUS::Success, result);
}
//
//  *******************************************************************************************************************
TEST_F(BTParallelTest, MultipleChildFailure)
{
	shared_ptr<cMockBTNode> pFailureNode(new cMockBTNode);
	EXPECT_CALL(*pFailureNode, VOnUpdate(NULL, 0.0f))
		.WillOnce(Return(BT_STATUS::Running))
		.WillOnce(Return(BT_STATUS::Running))
		.WillOnce(Return(BT_STATUS::Failure));

	shared_ptr<cMockBTNode> pSuccessNode1(new cMockBTNode);
	EXPECT_CALL(*pSuccessNode1, VOnUpdate(NULL, 0.0f))
		.WillRepeatedly(Return(BT_STATUS::Success));

	shared_ptr<cMockBTNode> pSuccessNode2(new cMockBTNode);
	EXPECT_CALL(*pSuccessNode2, VOnUpdate(NULL, 0.0f))
		.WillOnce(Return(BT_STATUS::Running))
		.WillOnce(Return(BT_STATUS::Success));

	shared_ptr<cBTParallelNode> pParallelNode = cBTNodeFactory::Create<cBTParallelNode>()
		->AddChild<cBTParallelNode>(pSuccessNode1)
		->AddChild<cBTParallelNode>(pFailureNode)
		->AddChild<cBTParallelNode>(pSuccessNode2);

	BTTreeStrongPtr pBT(DEBUG_NEW cBTBehaviorTree(pParallelNode));

	BT_STATUS::Enum result = pBT->Tick(NULL, 0.0f);
	EXPECT_EQ(BT_STATUS::Running, result);
	result = pBT->Tick(NULL, 0.0f);
	EXPECT_EQ(BT_STATUS::Running, result);
	result = pBT->Tick(NULL, 0.0f);
	EXPECT_EQ(BT_STATUS::Failure, result);;
}

//  *******************************************************************************************************************
TEST_F(BTParallelTest, MultipleChildSuccess)
{
	shared_ptr<cMockBTNode> pSuccessNode1(new cMockBTNode);
	EXPECT_CALL(*pSuccessNode1, VOnUpdate(NULL, 0.0f))
		.WillRepeatedly(Return(BT_STATUS::Success));

	shared_ptr<cMockBTNode> pSuccessNode2(new cMockBTNode);
	EXPECT_CALL(*pSuccessNode2, VOnUpdate(NULL, 0.0f))
		.WillOnce(Return(BT_STATUS::Running))
		.WillOnce(Return(BT_STATUS::Running))
		.WillOnce(Return(BT_STATUS::Success));

	shared_ptr<cBTParallelNode> pParallelNode = cBTNodeFactory::Create<cBTParallelNode>()
		->AddChild<cBTParallelNode>(pSuccessNode1)
		->AddChild<cBTParallelNode>(pSuccessNode2);

	BTTreeStrongPtr pBT(DEBUG_NEW cBTBehaviorTree(pParallelNode));

	BT_STATUS::Enum result;
	result = pBT->Tick(NULL, 0.0f);
	EXPECT_EQ(BT_STATUS::Running, result);
	result = pBT->Tick(NULL, 0.0f);
	EXPECT_EQ(BT_STATUS::Running, result);
	result = pBT->Tick(NULL, 0.0f);
	EXPECT_EQ(BT_STATUS::Success, result);;
}

//  *******************************************************************************************************************
TEST_F(BTParallelTest, RequireAllFailurePolicyLimbo)
{
	shared_ptr<cMockBTNode> pFailureNode(new cMockBTNode);
	EXPECT_CALL(*pFailureNode, VOnUpdate(NULL, 0.0f))
		.Times(AnyNumber())
		.WillOnce(Return(BT_STATUS::Running))
		.WillOnce(Return(BT_STATUS::Running))
		.WillOnce(Return(BT_STATUS::Failure))
		.WillOnce(Return(BT_STATUS::Running))
		.WillOnce(Return(BT_STATUS::Running))
		.WillOnce(Return(BT_STATUS::Failure));

	shared_ptr<cMockBTNode> pSuccessNode1(new cMockBTNode);
	EXPECT_CALL(*pSuccessNode1, VOnUpdate(NULL, 0.0f))
		.WillRepeatedly(Return(BT_STATUS::Failure));

	shared_ptr<cMockBTNode> pSuccessNode2(new cMockBTNode);
	EXPECT_CALL(*pSuccessNode2, VOnUpdate(NULL, 0.0f))
		.Times(AnyNumber())
		.WillOnce(Return(BT_STATUS::Running))
		.WillOnce(Return(BT_STATUS::Running))
		.WillOnce(Return(BT_STATUS::Success))
		.WillOnce(Return(BT_STATUS::Running))
		.WillOnce(Return(BT_STATUS::Running))
		.WillOnce(Return(BT_STATUS::Success));

	shared_ptr<cBTParallelNode> pParallelNode = cBTNodeFactory::Create<cBTParallelNode>()
		->SetFailurePolicy(BT_POLICY::RequireAll)
		->AddChild<cBTParallelNode>(pSuccessNode1)
		->AddChild<cBTParallelNode>(pFailureNode)
		->AddChild<cBTParallelNode>(pSuccessNode2);

	BTTreeStrongPtr pBT(DEBUG_NEW cBTBehaviorTree(pParallelNode));

	BT_STATUS::Enum result;
	for (int i=0; i<2; i++)
	{
		result = pBT->Tick(NULL, 0.0f);
		EXPECT_EQ(BT_STATUS::Running, result);
		result = pBT->Tick(NULL, 0.0f);
		EXPECT_EQ(BT_STATUS::Running, result);
		result = pBT->Tick(NULL, 0.0f);
		EXPECT_EQ(BT_STATUS::Running, result);;
	}
}

//  *******************************************************************************************************************
TEST_F(BTParallelTest, RequireAllFailurePolicy)
{
	shared_ptr<cMockBTNode> pFailureNode(new cMockBTNode);
	EXPECT_CALL(*pFailureNode, VOnUpdate(NULL, 0.0f))
		.Times(AnyNumber())
		.WillOnce(Return(BT_STATUS::Running))
		.WillOnce(Return(BT_STATUS::Running))
		.WillOnce(Return(BT_STATUS::Failure))
		.WillOnce(Return(BT_STATUS::Running))
		.WillOnce(Return(BT_STATUS::Running))
		.WillOnce(Return(BT_STATUS::Failure));

	shared_ptr<cMockBTNode> pSuccessNode1(new cMockBTNode);
	EXPECT_CALL(*pSuccessNode1, VOnUpdate(NULL, 0.0f))
		.WillRepeatedly(Return(BT_STATUS::Failure));

	shared_ptr<cMockBTNode> pSuccessNode2(new cMockBTNode);
	EXPECT_CALL(*pSuccessNode2, VOnUpdate(NULL, 0.0f))
		.Times(AnyNumber())
		.WillOnce(Return(BT_STATUS::Running))
		.WillOnce(Return(BT_STATUS::Running))
		.WillOnce(Return(BT_STATUS::Failure));

	shared_ptr<cBTParallelNode> pParallelNode = cBTNodeFactory::Create<cBTParallelNode>()
		->SetFailurePolicy(BT_POLICY::RequireAll)
		->AddChild<cBTParallelNode>(pSuccessNode1)
		->AddChild<cBTParallelNode>(pFailureNode)
		->AddChild<cBTParallelNode>(pSuccessNode2);

	BTTreeStrongPtr pBT(DEBUG_NEW cBTBehaviorTree(pParallelNode));

	BT_STATUS::Enum result;
	result = pBT->Tick(NULL, 0.0f);
	EXPECT_EQ(BT_STATUS::Running, result);
	result = pBT->Tick(NULL, 0.0f);
	EXPECT_EQ(BT_STATUS::Running, result);
	result = pBT->Tick(NULL, 0.0f);
	EXPECT_EQ(BT_STATUS::Failure, result);;
}

//  *******************************************************************************************************************
TEST_F(BTParallelTest, SucceedRequireOneFailureRequireOne)
{
	shared_ptr<cMockBTNode> pFailureNode(new cMockBTNode);
	EXPECT_CALL(*pFailureNode, VOnUpdate(NULL, 0.0f))
		.Times(AnyNumber())
		.WillOnce(Return(BT_STATUS::Running))
		.WillOnce(Return(BT_STATUS::Running))
		.WillOnce(Return(BT_STATUS::Failure))
		.WillOnce(Return(BT_STATUS::Running))
		.WillOnce(Return(BT_STATUS::Running))
		.WillOnce(Return(BT_STATUS::Failure));

	shared_ptr<cMockBTNode> pSuccessNode(new cMockBTNode);
	EXPECT_CALL(*pSuccessNode, VOnUpdate(NULL, 0.0f))
		.Times(AnyNumber())
		.WillOnce(Return(BT_STATUS::Running))
		.WillRepeatedly(Return(BT_STATUS::Success));

	shared_ptr<cBTParallelNode> pParallelNode = cBTNodeFactory::Create<cBTParallelNode>()
		->SetSuccessPolicy(BT_POLICY::RequireOne)
		->AddChild<cBTParallelNode>(pFailureNode)
		->AddChild<cBTParallelNode>(pSuccessNode);

	BTTreeStrongPtr pBT(DEBUG_NEW cBTBehaviorTree(pParallelNode));

	BT_STATUS::Enum result;
	result = pBT->Tick(NULL, 0.0f);
	EXPECT_EQ(BT_STATUS::Running, result);
	result = pBT->Tick(NULL, 0.0f);
	EXPECT_EQ(BT_STATUS::Running, result);
	result = pBT->Tick(NULL, 0.0f);
	EXPECT_EQ(BT_STATUS::Failure, result);;
}

//  *******************************************************************************************************************
TEST_F(BTParallelTest, SucceedRequireOneFailureRequireAll)
{
	shared_ptr<cMockBTNode> pFailureNode(new cMockBTNode);
	EXPECT_CALL(*pFailureNode, VOnUpdate(NULL, 0.0f))
		.Times(AnyNumber())
		.WillOnce(Return(BT_STATUS::Running))
		.WillOnce(Return(BT_STATUS::Running))
		.WillOnce(Return(BT_STATUS::Success))
		.WillOnce(Return(BT_STATUS::Running))
		.WillOnce(Return(BT_STATUS::Running))
		.WillOnce(Return(BT_STATUS::Success));

	shared_ptr<cMockBTNode> pSuccessNode(new cMockBTNode);
	EXPECT_CALL(*pSuccessNode, VOnUpdate(NULL, 0.0f))
		.Times(AnyNumber())
		.WillOnce(Return(BT_STATUS::Running))
		.WillRepeatedly(Return(BT_STATUS::Failure));

	shared_ptr<cBTParallelNode> pParallelNode = cBTNodeFactory::Create<cBTParallelNode>()
		->SetSuccessPolicy(BT_POLICY::RequireOne)
		->SetFailurePolicy(BT_POLICY::RequireAll)
		->AddChild<cBTParallelNode>(pFailureNode)
		->AddChild<cBTParallelNode>(pSuccessNode);

	BTTreeStrongPtr pBT(DEBUG_NEW cBTBehaviorTree(pParallelNode));

	BT_STATUS::Enum result;
	result = pBT->Tick(NULL, 0.0f);
	EXPECT_EQ(BT_STATUS::Running, result);
	result = pBT->Tick(NULL, 0.0f);
	EXPECT_EQ(BT_STATUS::Running, result);
	result = pBT->Tick(NULL, 0.0f);
	EXPECT_EQ(BT_STATUS::Success, result);;
}
