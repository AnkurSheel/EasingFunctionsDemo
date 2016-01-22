#include "stdafx.h"
#include "Any.h"
#include "myString.h"

using namespace Base;

class AnyTest : public ::testing::Test
{
  void SetUp() 
  {
    m_strTestData.Set(cString("test"));
    m_intTestData.Set(5);
  }
  void TearDown() {}

public:
  Any m_strTestData;
  Any m_intTestData;
};

TEST_F(AnyTest, Initial)
{
  Any testData;
  EXPECT_FALSE(testData.IsValid());
  EXPECT_FALSE(!testData);
}

TEST_F(AnyTest, ValueConstructionSuccess)
{
  Any testData1(cString("test"));
  Any testData2(5);

  EXPECT_TRUE(testData1.IsValid());
  EXPECT_TRUE(!testData2);

  EXPECT_STREQ((*(testData1.Get<cString>())).GetData(), "test");
  EXPECT_EQ((*(testData2.Get<int32>())), 5);
}

TEST_F(AnyTest, CopyConstructionSuccess)
{
  Any testData1(m_strTestData);
  Any testData2(m_intTestData);

  EXPECT_TRUE(testData1.IsValid());
  EXPECT_TRUE(!testData2);

  EXPECT_STREQ((*(testData1.Get<cString>())).GetData(), "test");
  EXPECT_EQ((*(testData2.Get<int32>())), 5);
}

TEST_F(AnyTest, AssignmentOperatorSuccess)
{
  Any testData1 = m_strTestData;
  Any testData2 = m_intTestData;

  EXPECT_TRUE(testData1.IsValid());
  EXPECT_TRUE(!testData2);

  EXPECT_STREQ((*(testData1.Get<cString>())).GetData(), "test");
  EXPECT_EQ((*(testData2.Get<int32>())), 5);
}

TEST_F(AnyTest, Is)
{
  EXPECT_TRUE(m_strTestData.Is<cString>());
  EXPECT_FALSE(m_strTestData.Is<uint32>());

  EXPECT_TRUE(m_intTestData.Is<int32>());
  EXPECT_FALSE(m_intTestData.Is<cString>());
  EXPECT_FALSE(m_intTestData.Is<int16>());
}

TEST_F(AnyTest, Compare)
{
  Any testData1(cString("test"));
  Any testData2(5);
  Any testData3(cString("test1"));
  Any testData4(6);

  EXPECT_TRUE(m_strTestData.Compare(testData1));
  EXPECT_TRUE(m_intTestData.Compare(testData2));

  EXPECT_FALSE(m_strTestData.Compare(m_intTestData));
  EXPECT_FALSE(m_intTestData.Compare(m_strTestData));

  EXPECT_FALSE(m_strTestData.Compare(testData3));
  EXPECT_FALSE(testData2.Compare(testData4));
}
