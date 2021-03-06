#include "stdafx.h"
#include "ServiceLocator.h"

using namespace Base;

class IFooBar {
public:
	virtual ~IFooBar() {};
	virtual cString VGetName() = 0;
};

class Foo : public IFooBar 
{
public:
	cString VGetName() OVERRIDE
	{
		return "Foo";
	}
};

class Bar: public IFooBar 
{
public:
	cString VGetName() OVERRIDE
	{
		return "Bar";
	}
};

class ServiceLocatorTest : public ::testing::Test
{
protected:

	void SetUp()
	{
	}

	void TearDown()
	{
		cServiceLocator::Destroy();
	}
};

TEST_F(ServiceLocatorTest, GetServiceFail)
{
	const shared_ptr<IFooBar> pFoo = MakeStrongPtr<IFooBar>(cServiceLocator::GetInstance()->GetService<IFooBar>());
	EXPECT_EQ(NULL, pFoo.get());
}

TEST_F(ServiceLocatorTest, GetServiceSuccess)
{
	cServiceLocator::GetInstance()->Register<IFooBar>(shared_ptr<IFooBar>(new Foo()));
	const shared_ptr<IFooBar> pFoo = MakeStrongPtr<IFooBar>(cServiceLocator::GetInstance()->GetService<IFooBar>());
	EXPECT_STREQ("Foo", pFoo->VGetName().GetData());

  cServiceLocator::Destroy();

	cServiceLocator::GetInstance()->Register<IFooBar>(shared_ptr<IFooBar>(new Bar()));
	const shared_ptr<IFooBar> pBar = MakeStrongPtr<IFooBar>(cServiceLocator::GetInstance()->GetService<IFooBar>());
	EXPECT_STREQ("Bar", pBar->VGetName().GetData());
}
