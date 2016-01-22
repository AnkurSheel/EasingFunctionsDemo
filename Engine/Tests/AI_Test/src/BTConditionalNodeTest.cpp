#include "stdafx.h"
#include "BTConditionalNode.h"
#include "BTNodeFactory.h"
#include "BtBehaviorTree.h"

using namespace AI;
using namespace Base;

class TestClass
{
public:
	TestClass()
		: IntValue(0)
		, BoolValue(true)
		, FloatValue(0.0f)
	{

	};
	int ReturnInt() const {return IntValue;};
	bool ReturnBool() const {return BoolValue;};
	float ReturnFloat() const {return FloatValue;};

public:
	int IntValue;
	bool BoolValue;
	float FloatValue;
};

class BTConditionalNodeTest : public ::testing::Test
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

AutoCaptureOutput * BTConditionalNodeTest::m_pOutput;

//  *******************************************************************************************************************
TEST_F(BTConditionalNodeTest, BoolSuccess)
{
	shared_ptr<TestClass> pClass(new TestClass());
	pClass->BoolValue = true;

	shared_ptr<cBTConditionalNode<bool, TestClass> > pBoolConditionalNode = cBTNodeFactory::Create<cBTConditionalNode<bool, TestClass> >()
		->SetHolder(pClass)
		->SetFunction(&TestClass::ReturnBool)
		->SetValue(true);

	BTTreeStrongPtr pBT(DEBUG_NEW cBTBehaviorTree(pBoolConditionalNode));

	BT_STATUS::Enum status = pBT->Tick(NULL, 0.0f);
	EXPECT_EQ(BT_STATUS::Success, status);

	pBoolConditionalNode.reset();

	pClass->BoolValue = false;
	pBoolConditionalNode = cBTNodeFactory::Create<cBTConditionalNode<bool, TestClass> >()
		->SetHolder(pClass)
		->SetFunction(&TestClass::ReturnBool)
		->SetValue(false);

	pBT->SetRoot(pBoolConditionalNode);

	status = pBT->Tick(NULL, 0.0f);
	EXPECT_EQ(BT_STATUS::Success, status);
}

//  *******************************************************************************************************************
TEST_F(BTConditionalNodeTest, BoolFailure)
{
	shared_ptr<TestClass> pClass1(new TestClass());
	pClass1->BoolValue = true;

	shared_ptr<cBTConditionalNode<bool, TestClass> > pBoolConditionalNode = cBTNodeFactory::Create<cBTConditionalNode<bool, TestClass> >()
		->SetHolder(pClass1)
		->SetFunction(&TestClass::ReturnBool)
		->SetValue(false);

	BTTreeStrongPtr pBT(DEBUG_NEW cBTBehaviorTree(pBoolConditionalNode));

	BT_STATUS::Enum status = pBT->Tick(NULL, 0.0f);
	EXPECT_EQ(BT_STATUS::Failure, status);

	shared_ptr<TestClass> pClass2(new TestClass());
	pClass2->BoolValue = false;

	pBoolConditionalNode.reset();

	pBoolConditionalNode = cBTNodeFactory::Create<cBTConditionalNode<bool, TestClass> >()
		->SetHolder(pClass2)
		->SetFunction(&TestClass::ReturnBool)
		->SetValue(true);

	status = pBT->Tick(NULL, 0.0f);
	EXPECT_EQ(BT_STATUS::Failure, status);
}

//  *******************************************************************************************************************
TEST_F(BTConditionalNodeTest, IntCheckEqual)
{
	shared_ptr<TestClass> pClass1(new TestClass());
	pClass1->IntValue = 5;

	shared_ptr<cBTConditionalNode<int, TestClass> > pIntConditionalNode = cBTNodeFactory::Create<cBTConditionalNode<int, TestClass> >()
		->SetHolder(pClass1)
		->SetFunction(&TestClass::ReturnInt)
		->SetTestType(ConditionalOperators::Equal)
		->SetValue(5);

	BTTreeStrongPtr pBT(DEBUG_NEW cBTBehaviorTree(pIntConditionalNode));

	BT_STATUS::Enum status = pBT->Tick(NULL, 0.0f);
	EXPECT_EQ(BT_STATUS::Success, status);

	shared_ptr<TestClass> pClass2(new TestClass());
	pClass2->IntValue = 10;

	pIntConditionalNode.reset();

	pIntConditionalNode = cBTNodeFactory::Create<cBTConditionalNode<int, TestClass> >()
		->SetHolder(pClass2)
		->SetFunction(&TestClass::ReturnInt)
		->SetTestType(ConditionalOperators::Equal)
		->SetValue(20);

	pBT->SetRoot(pIntConditionalNode);

	status = pBT->Tick(NULL, 0.0f);
	EXPECT_EQ(BT_STATUS::Failure, status);
}

//  *******************************************************************************************************************
TEST_F(BTConditionalNodeTest, IntCheckNotEquals)
{
	shared_ptr<TestClass> pClass1(new TestClass());
	pClass1->IntValue = 5;

	shared_ptr<cBTConditionalNode<int, TestClass> > pIntConditionalNode = cBTNodeFactory::Create<cBTConditionalNode<int, TestClass> >()
		->SetHolder(pClass1)
		->SetFunction(&TestClass::ReturnInt)
		->SetTestType(ConditionalOperators::NotEqual)
		->SetValue(5);

	BTTreeStrongPtr pBT(DEBUG_NEW cBTBehaviorTree(pIntConditionalNode));

	BT_STATUS::Enum status = pBT->Tick(NULL, 0.0f);
	EXPECT_EQ(BT_STATUS::Failure, status);

	shared_ptr<TestClass> pClass2(new TestClass());
	pClass2->IntValue = 10;

	pIntConditionalNode.reset();

	pIntConditionalNode = cBTNodeFactory::Create<cBTConditionalNode<int, TestClass> >()
		->SetHolder(pClass2)
		->SetFunction(&TestClass::ReturnInt)
		->SetTestType(ConditionalOperators::NotEqual)
		->SetValue(20);

	pBT->SetRoot(pIntConditionalNode);

	status = pBT->Tick(NULL, 0.0f);
	EXPECT_EQ(BT_STATUS::Success, status);
}

//  *******************************************************************************************************************
TEST_F(BTConditionalNodeTest, IntCheckGreater)
{
	shared_ptr<TestClass> pClass1(new TestClass());
	pClass1->IntValue = 5;

	shared_ptr<cBTConditionalNode<int, TestClass> > pIntConditionalNode = cBTNodeFactory::Create<cBTConditionalNode<int, TestClass> >()
		->SetHolder(pClass1)
		->SetFunction(&TestClass::ReturnInt)
		->SetTestType(ConditionalOperators::GreaterThan)
		->SetValue(2);

	BTTreeStrongPtr pBT(DEBUG_NEW cBTBehaviorTree(pIntConditionalNode));

	BT_STATUS::Enum status = pBT->Tick(NULL, 0.0f);
	EXPECT_EQ(BT_STATUS::Success, status);

	shared_ptr<TestClass> pClass2(new TestClass());
	pClass2->IntValue = 10;

	pIntConditionalNode.reset();

	pIntConditionalNode = cBTNodeFactory::Create<cBTConditionalNode<int, TestClass> >()
		->SetHolder(pClass2)
		->SetFunction(&TestClass::ReturnInt)
		->SetTestType(ConditionalOperators::GreaterThan)
		->SetValue(10);

	pBT->SetRoot(pIntConditionalNode);

	status = pBT->Tick(NULL, 0.0f);
	EXPECT_EQ(BT_STATUS::Failure, status);
}

//  *******************************************************************************************************************
TEST_F(BTConditionalNodeTest, IntCheckGreaterOrEqual)
{
	shared_ptr<TestClass> pClass1(new TestClass());
	pClass1->IntValue = 5;

	shared_ptr<cBTConditionalNode<int, TestClass> > pIntConditionalNode = cBTNodeFactory::Create<cBTConditionalNode<int, TestClass> >()
		->SetHolder(pClass1)
		->SetFunction(&TestClass::ReturnInt)
		->SetTestType(ConditionalOperators::GreaterOrEqual)
		->SetValue(2);

	BTTreeStrongPtr pBT(DEBUG_NEW cBTBehaviorTree(pIntConditionalNode));

	BT_STATUS::Enum status = pBT->Tick(NULL, 0.0f);
	EXPECT_EQ(BT_STATUS::Success, status);

	shared_ptr<TestClass> pClass2(new TestClass());
	pClass2->IntValue = 10;

	pIntConditionalNode.reset();

	pIntConditionalNode = cBTNodeFactory::Create<cBTConditionalNode<int, TestClass> >()
		->SetHolder(pClass2)
		->SetFunction(&TestClass::ReturnInt)
		->SetTestType(ConditionalOperators::GreaterOrEqual)
		->SetValue(10);

	status = pBT->Tick(NULL, 0.0f);
	EXPECT_EQ(BT_STATUS::Success, status);

	pIntConditionalNode.reset();

	pIntConditionalNode = cBTNodeFactory::Create<cBTConditionalNode<int, TestClass> >()
		->SetHolder(pClass2)
		->SetFunction(&TestClass::ReturnInt)
		->SetTestType(ConditionalOperators::GreaterOrEqual)
		->SetValue(20);

	pBT->SetRoot(pIntConditionalNode);

	status = pBT->Tick(NULL, 0.0f);
	EXPECT_EQ(BT_STATUS::Failure, status);
}

//  *******************************************************************************************************************
TEST_F(BTConditionalNodeTest, IntCheckLesser)
{
	shared_ptr<TestClass> pClass1(new TestClass());
	pClass1->IntValue = 5;

	shared_ptr<cBTConditionalNode<int, TestClass> > pIntConditionalNode = cBTNodeFactory::Create<cBTConditionalNode<int, TestClass> >()
		->SetHolder(pClass1)
		->SetFunction(&TestClass::ReturnInt)
		->SetTestType(ConditionalOperators::LessThan)
		->SetValue(10);

	BTTreeStrongPtr pBT(DEBUG_NEW cBTBehaviorTree(pIntConditionalNode));

	BT_STATUS::Enum status = pBT->Tick(NULL, 0.0f);
	EXPECT_EQ(BT_STATUS::Success, status);

	shared_ptr<TestClass> pClass2(new TestClass());
	pClass2->IntValue = 10;

	pIntConditionalNode.reset();

	pIntConditionalNode = cBTNodeFactory::Create<cBTConditionalNode<int, TestClass> >()
		->SetHolder(pClass2)
		->SetFunction(&TestClass::ReturnInt)
		->SetTestType(ConditionalOperators::LessThan)
		->SetValue(10);

	pBT->SetRoot(pIntConditionalNode);

	status = pBT->Tick(NULL, 0.0f);
	EXPECT_EQ(BT_STATUS::Failure, status);

	pIntConditionalNode.reset();

	pIntConditionalNode = cBTNodeFactory::Create<cBTConditionalNode<int, TestClass> >()
		->SetHolder(pClass2)
		->SetFunction(&TestClass::ReturnInt)
		->SetTestType(ConditionalOperators::LessThan)
		->SetValue(-1);

	pBT->SetRoot(pIntConditionalNode);

	status = pBT->Tick(NULL, 0.0f);
	EXPECT_EQ(BT_STATUS::Failure, status);
}

//  *******************************************************************************************************************
TEST_F(BTConditionalNodeTest, IntCheckLesserOrEqual)
{
	shared_ptr<TestClass> pClass1(new TestClass());
	pClass1->IntValue = 5;

	shared_ptr<cBTConditionalNode<int, TestClass> > pIntConditionalNode = cBTNodeFactory::Create<cBTConditionalNode<int, TestClass> >()
		->SetHolder(pClass1)
		->SetFunction(&TestClass::ReturnInt)
		->SetTestType(ConditionalOperators::LessOrEqual)
		->SetValue(10);

	BTTreeStrongPtr pBT(DEBUG_NEW cBTBehaviorTree(pIntConditionalNode));

	BT_STATUS::Enum status = pBT->Tick(NULL, 0.0f);
	EXPECT_EQ(BT_STATUS::Success, status);

	shared_ptr<TestClass> pClass2(new TestClass());
	pClass2->IntValue = 10;

	pIntConditionalNode.reset();

	pIntConditionalNode = cBTNodeFactory::Create<cBTConditionalNode<int, TestClass> >()
		->SetHolder(pClass2)
		->SetFunction(&TestClass::ReturnInt)
		->SetTestType(ConditionalOperators::LessOrEqual)
		->SetValue(10);

	status = pBT->Tick(NULL, 0.0f);
	EXPECT_EQ(BT_STATUS::Success, status);

	pIntConditionalNode.reset();

	pIntConditionalNode = cBTNodeFactory::Create<cBTConditionalNode<int, TestClass> >()
		->SetHolder(pClass2)
		->SetFunction(&TestClass::ReturnInt)
		->SetTestType(ConditionalOperators::LessOrEqual)
		->SetValue(2);

	pBT->SetRoot(pIntConditionalNode);

	status = pBT->Tick(NULL, 0.0f);
	EXPECT_EQ(BT_STATUS::Failure, status);
}

//  *******************************************************************************************************************
TEST_F(BTConditionalNodeTest, FloatCheckEqual)
{
	shared_ptr<TestClass> pClass1(new TestClass());
	pClass1->FloatValue = 5.0f;

	shared_ptr<cBTConditionalNode<float, TestClass> > pFloatConditionalNode = cBTNodeFactory::Create<cBTConditionalNode<float, TestClass> >()
		->SetHolder(pClass1)
		->SetFunction(&TestClass::ReturnFloat)
		->SetTestType(ConditionalOperators::Equal)
		->SetValue(5.0f);

	BTTreeStrongPtr pBT(DEBUG_NEW cBTBehaviorTree(pFloatConditionalNode));

	BT_STATUS::Enum status = pBT->Tick(NULL, 0.0f);
	EXPECT_EQ(BT_STATUS::Success, status);

	shared_ptr<TestClass> pClass2(new TestClass());
	pClass2->FloatValue = 10.0f;

	pFloatConditionalNode.reset();

	pFloatConditionalNode = cBTNodeFactory::Create<cBTConditionalNode<float, TestClass> >()
		->SetHolder(pClass2)
		->SetFunction(&TestClass::ReturnFloat)
		->SetTestType(ConditionalOperators::Equal)
		->SetValue(20.0f);

	pBT->SetRoot(pFloatConditionalNode);

	status = pBT->Tick(NULL, 0.0f);
	EXPECT_EQ(BT_STATUS::Failure, status);
}

//  *******************************************************************************************************************
TEST_F(BTConditionalNodeTest, FloatCheckNotEquals)
{
	shared_ptr<TestClass> pClass1(new TestClass());
	pClass1->FloatValue = 5.0f;

	shared_ptr<cBTConditionalNode<float, TestClass> > pFloatConditionalNode = cBTNodeFactory::Create<cBTConditionalNode<float, TestClass> >()
		->SetHolder(pClass1)
		->SetFunction(&TestClass::ReturnFloat)
		->SetTestType(ConditionalOperators::NotEqual)
		->SetValue(5.0f);

	BTTreeStrongPtr pBT(DEBUG_NEW cBTBehaviorTree(pFloatConditionalNode));

	BT_STATUS::Enum status = pBT->Tick(NULL, 0.0f);
	EXPECT_EQ(BT_STATUS::Failure, status);

	shared_ptr<TestClass> pClass2(new TestClass());
	pClass2->FloatValue = 10.0f;

	pFloatConditionalNode.reset();

	pFloatConditionalNode = cBTNodeFactory::Create<cBTConditionalNode<float, TestClass> >()
		->SetHolder(pClass2)
		->SetFunction(&TestClass::ReturnFloat)
		->SetTestType(ConditionalOperators::NotEqual)
		->SetValue(20.0f);

	pBT->SetRoot(pFloatConditionalNode);

	status = pBT->Tick(NULL, 0.0f);
	EXPECT_EQ(BT_STATUS::Success, status);

	pFloatConditionalNode.reset();

	pFloatConditionalNode = cBTNodeFactory::Create<cBTConditionalNode<float, TestClass> >()
		->SetHolder(pClass2)
		->SetFunction(&TestClass::ReturnFloat)
		->SetTestType(ConditionalOperators::NotEqual)
		->SetValue(10.00001f);

	pBT->SetRoot(pFloatConditionalNode);

	status = pBT->Tick(NULL, 0.0f);
	EXPECT_EQ(BT_STATUS::Failure, status);
}

//  *******************************************************************************************************************
TEST_F(BTConditionalNodeTest, FloatCheckGreater)
{
	shared_ptr<TestClass> pClass1(new TestClass());
	pClass1->FloatValue = 5.0f;

	shared_ptr<cBTConditionalNode<float, TestClass> > pFloatConditionalNode = cBTNodeFactory::Create<cBTConditionalNode<float, TestClass> >()
		->SetHolder(pClass1)
		->SetFunction(&TestClass::ReturnFloat)
		->SetTestType(ConditionalOperators::GreaterThan)
		->SetValue(2.0f);

	BTTreeStrongPtr pBT(DEBUG_NEW cBTBehaviorTree(pFloatConditionalNode));

	BT_STATUS::Enum status = pBT->Tick(NULL, 0.0f);
	EXPECT_EQ(BT_STATUS::Success, status);

	shared_ptr<TestClass> pClass2(new TestClass());
	pClass2->FloatValue = 10.0f;

	pFloatConditionalNode.reset();

	pFloatConditionalNode = cBTNodeFactory::Create<cBTConditionalNode<float, TestClass> >()
		->SetHolder(pClass2)
		->SetFunction(&TestClass::ReturnFloat)
		->SetTestType(ConditionalOperators::GreaterThan)
		->SetValue(10.0f);

	pBT->SetRoot(pFloatConditionalNode);

	status = pBT->Tick(NULL, 0.0f);
	EXPECT_EQ(BT_STATUS::Failure, status);
}

//  *******************************************************************************************************************
TEST_F(BTConditionalNodeTest, FloatCheckGreaterOrEqual)
{
	shared_ptr<TestClass> pClass1(new TestClass());
	pClass1->FloatValue = 5.0f;

	shared_ptr<cBTConditionalNode<float, TestClass> > pFloatConditionalNode = cBTNodeFactory::Create<cBTConditionalNode<float, TestClass> >()
		->SetHolder(pClass1)
		->SetFunction(&TestClass::ReturnFloat)
		->SetTestType(ConditionalOperators::GreaterOrEqual)
		->SetValue(2.0f);

	BTTreeStrongPtr pBT(DEBUG_NEW cBTBehaviorTree(pFloatConditionalNode));

	BT_STATUS::Enum status = pBT->Tick(NULL, 0.0f);
	EXPECT_EQ(BT_STATUS::Success, status);

	shared_ptr<TestClass> pClass2(new TestClass());
	pClass2->FloatValue = 10.0f;

	pFloatConditionalNode.reset();

	pFloatConditionalNode = cBTNodeFactory::Create<cBTConditionalNode<float, TestClass> >()
		->SetHolder(pClass2)
		->SetFunction(&TestClass::ReturnFloat)
		->SetTestType(ConditionalOperators::GreaterOrEqual)
		->SetValue(10.0f);

	status = pBT->Tick(NULL, 0.0f);
	EXPECT_EQ(BT_STATUS::Success, status);

	pFloatConditionalNode.reset();

	pFloatConditionalNode = cBTNodeFactory::Create<cBTConditionalNode<float, TestClass> >()
		->SetHolder(pClass2)
		->SetFunction(&TestClass::ReturnFloat)
		->SetTestType(ConditionalOperators::GreaterOrEqual)
		->SetValue(5.00001f);

	status = pBT->Tick(NULL, 0.0f);
	EXPECT_EQ(BT_STATUS::Success, status);
}

//  *******************************************************************************************************************
TEST_F(BTConditionalNodeTest, FloatCheckLesser)
{
	shared_ptr<TestClass> pClass1(new TestClass());
	pClass1->FloatValue = 5.0f;

	shared_ptr<cBTConditionalNode<float, TestClass> > pFloatConditionalNode = cBTNodeFactory::Create<cBTConditionalNode<float, TestClass> >()
		->SetHolder(pClass1)
		->SetFunction(&TestClass::ReturnFloat)
		->SetTestType(ConditionalOperators::LessThan)
		->SetValue(10.0f);

	BTTreeStrongPtr pBT(DEBUG_NEW cBTBehaviorTree(pFloatConditionalNode));

	BT_STATUS::Enum status = pBT->Tick(NULL, 0.0f);
	EXPECT_EQ(BT_STATUS::Success, status);

	shared_ptr<TestClass> pClass2(new TestClass());
	pClass2->FloatValue = 10.0f;

	pFloatConditionalNode.reset();

	pFloatConditionalNode = cBTNodeFactory::Create<cBTConditionalNode<float, TestClass> >()
		->SetHolder(pClass2)
		->SetFunction(&TestClass::ReturnFloat)
		->SetTestType(ConditionalOperators::LessThan)
		->SetValue(10.0f);

	pBT->SetRoot(pFloatConditionalNode);

	status = pBT->Tick(NULL, 0.0f);
	EXPECT_EQ(BT_STATUS::Failure, status);

	pFloatConditionalNode.reset();

	pFloatConditionalNode = cBTNodeFactory::Create<cBTConditionalNode<float, TestClass> >()
		->SetHolder(pClass2)
		->SetFunction(&TestClass::ReturnFloat)
		->SetTestType(ConditionalOperators::LessThan)
		->SetValue(-1.0f);

	pBT->SetRoot(pFloatConditionalNode);

	status = pBT->Tick(NULL, 0.0f);
	EXPECT_EQ(BT_STATUS::Failure, status);
}

//  *******************************************************************************************************************
TEST_F(BTConditionalNodeTest, FloatCheckLesserOrEqual)
{
	shared_ptr<TestClass> pClass1(new TestClass());
	pClass1->FloatValue = 5.0f;

	shared_ptr<cBTConditionalNode<float, TestClass> > pFloatConditionalNode = cBTNodeFactory::Create<cBTConditionalNode<float, TestClass> >()
		->SetHolder(pClass1)
		->SetFunction(&TestClass::ReturnFloat)
		->SetTestType(ConditionalOperators::LessOrEqual)
		->SetValue(10.0f);

	BTTreeStrongPtr pBT(DEBUG_NEW cBTBehaviorTree(pFloatConditionalNode));

	BT_STATUS::Enum status = pBT->Tick(NULL, 0.0f);
	EXPECT_EQ(BT_STATUS::Success, status);

	shared_ptr<TestClass> pClass2(new TestClass());
	pClass2->FloatValue = 10.0f;

	pFloatConditionalNode.reset();

	pFloatConditionalNode = cBTNodeFactory::Create<cBTConditionalNode<float, TestClass> >()
		->SetHolder(pClass2)
		->SetFunction(&TestClass::ReturnFloat)
		->SetTestType(ConditionalOperators::LessOrEqual)
		->SetValue(10.0f);

	status = pBT->Tick(NULL, 0.0f);
	EXPECT_EQ(BT_STATUS::Success, status);

	pFloatConditionalNode.reset();

	pFloatConditionalNode = cBTNodeFactory::Create<cBTConditionalNode<float, TestClass> >()
		->SetHolder(pClass2)
		->SetFunction(&TestClass::ReturnFloat)
		->SetTestType(ConditionalOperators::LessOrEqual)
		->SetValue(4.0f);

	pBT->SetRoot(pFloatConditionalNode);

	status = pBT->Tick(NULL, 0.0f);
	EXPECT_EQ(BT_STATUS::Failure, status);
}
