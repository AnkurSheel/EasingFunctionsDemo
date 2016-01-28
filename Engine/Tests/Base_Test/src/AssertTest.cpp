#include "stdafx.h"
#include "SpAssert.h"
#include "SpAssertHandler.h"
#include "BaseUtilities.h"
#include "SpAssertSettings.hxx"
#include "Optional.h"
#include "SpAssertFwd.h"
#include "SpAssertContext.hxx"
#include "FileIO.h"
#include "SpAssertLogger.hxx"
#include "ServiceLocator.h"

using namespace Base;
using namespace std;

class cTestHandler : public cSpAssertHandler
{
public:
  cTestHandler()
    : m_pContext(NULL)
    , m_Action(AssertAction::None)
  {
  }

  ~cTestHandler() { SafeDelete(&m_pContext); }
  void Reset()
  {
    m_Action = AssertAction::None;
    SafeDelete(&m_pContext);
  }

public:
  const ISpAssertContext* m_pContext;
  AssertAction::AssertAction m_Action;

private:
  void VHandleAssert(const ISpAssertContext* const pContext) OVERRIDE
  {
    if (pContext != NULL)
    {
      m_pContext = ISpAssertContext::Duplicate(pContext);
    }

    const shared_ptr<ISpAssertSettings> pSettings =
        MakeStrongPtr<ISpAssertSettings>(cServiceLocator::GetInstance()->GetService<ISpAssertSettings>());

    switch (m_Action)
    {
      case AssertAction::IgnoreAll:
        if (pSettings != NULL)
        {
          pSettings->VSetIgnoreAll(true);
        }
        break;

      case AssertAction::IgnoreForever:
        if (pSettings != NULL)
        {
          pSettings->VAddToIgnoreList(pContext);
        }
        break;

      case AssertAction::None:
      case AssertAction::Ignore:
      default:
        break;
    }
  }
};

class cTestLogger : public ISpAssertLogger
{
public:
  cTestLogger()
    : m_pFile(nullptr)
  {
  }

  ~cTestLogger() {}
private:
  void VInitialize()
  {
    m_pFile = cFileIO::CreateFileIO();
    m_pFile->OpenFile("asserttest.log", ios_base::out);
  }

  void VLog(const ISpAssertContext* const pContext)
  {
    if (pContext != NULL && m_pFile != NULL)
    {
      cString strChooseMsg;
      strChooseMsg = "Assertion Failed : [" + pContext->VGetExpression();
      strChooseMsg += "] in : file: " + pContext->VGetFileName();
      strChooseMsg += " : line: " + cStringUtilities::MakeFormatted("%d", pContext->VGetLineNumber());
      if (!pContext->VGetCustomMessage().IsEmpty())
      {
        strChooseMsg += ": Message : " + pContext->VGetCustomMessage();
      }
      strChooseMsg += "\nValues:\n";

      for (auto iter = pContext->VGetVariablesArray().begin(); iter != pContext->VGetVariablesArray().end(); ++iter)
      {
        strChooseMsg += "\n" + iter->m_VariableName + " = " + iter->m_VariableValue;
      }
      m_pFile->WriteLine(strChooseMsg);
      m_pFile->Flush();
    }
  }

private:
  unique_ptr<cFileIO> m_pFile;
};

class AssertTest : public ::testing::Test
{
protected:
  void SetUp()
  {
    ISpAssertSettings::CreateAsService();

    const shared_ptr<ISpAssertSettings> pAssertSettings(
        MakeStrongPtr<ISpAssertSettings>(cServiceLocator::GetInstance()->GetService<ISpAssertSettings>()));

    unique_ptr<cTestHandler> pTestHandler(new cTestHandler());
    pAssertSettings->VSetHandler(std::move(pTestHandler));
  }
  void TearDown() { cServiceLocator::Destroy(); }
};

TEST_F(AssertTest, TestTrueConditions)
{
  shared_ptr<cTestHandler> pTestHandler(new cTestHandler());

  SP_ASSERT(true);
}

TEST_F(AssertTest, TestFile)
{
  const shared_ptr<ISpAssertSettings> pAssertSettings(
      MakeStrongPtr<ISpAssertSettings>(cServiceLocator::GetInstance()->GetService<ISpAssertSettings>()));

  int a = 5;
  int b = 6;
  SP_ASSERT(a == b);

  cString fileName = dynamic_cast<const cTestHandler*>(pAssertSettings->VGetHandler().get())->m_pContext->VGetFileName();
  tOptional<int> index = fileName.FindLastOfAny("\\", fileName.GetLength());
  if (index.IsValid())
  {
    fileName = fileName.GetSubString(index.GetValue() + 1, fileName.GetLength());
  }

  EXPECT_STREQ("asserttest.cpp", fileName.GetData());
}

TEST_F(AssertTest, TestLine)
{
  const shared_ptr<ISpAssertSettings> pAssertSettings(
      MakeStrongPtr<ISpAssertSettings>(cServiceLocator::GetInstance()->GetService<ISpAssertSettings>()));

  int a = 5;
  int b = 6;
  SP_ASSERT(a == b);
  EXPECT_EQ(SP_LINE - 1, dynamic_cast<const cTestHandler*>(pAssertSettings->VGetHandler().get())->m_pContext->VGetLineNumber());
}

TEST_F(AssertTest, TestFunction)
{
  const shared_ptr<ISpAssertSettings> pAssertSettings(
      MakeStrongPtr<ISpAssertSettings>(cServiceLocator::GetInstance()->GetService<ISpAssertSettings>()));

  int a = 5;
  int b = 6;
  SP_ASSERT(a == b);
  EXPECT_STREQ("AssertTest_TestFunction_Test::TestBody",
               dynamic_cast<const cTestHandler*>(pAssertSettings->VGetHandler().get())
                   ->m_pContext->VGetFunctionName()
                   .GetData());
}

TEST_F(AssertTest, TestExpression)
{
  const shared_ptr<ISpAssertSettings> pAssertSettings(
      MakeStrongPtr<ISpAssertSettings>(cServiceLocator::GetInstance()->GetService<ISpAssertSettings>()));

  cTestHandler* pTestHandler = const_cast<cTestHandler*>(dynamic_cast<const cTestHandler*>(pAssertSettings->VGetHandler().get()));

  int a = 5;
  int b = 6;
  SP_ASSERT(a == b);
  EXPECT_STREQ("a == b", pTestHandler->m_pContext->VGetExpression().GetData());

  // cppcheck-suppress unreadVariable
  int* p = NULL;
  pTestHandler->Reset();
  SP_ASSERT(p)(p);
  EXPECT_STREQ("p", pTestHandler->m_pContext->VGetExpression().GetData());

  pTestHandler->Reset();
  SP_ASSERT(a == b)(a)(b);
  EXPECT_STREQ("a == b", pTestHandler->m_pContext->VGetExpression().GetData());

  cString s1 = "ankur";
  cString s2 = "abc";

  pTestHandler->Reset();
  SP_ASSERT(s1.IsEmpty() && s2.IsEmpty())(s1)(s2);
  EXPECT_STREQ("s1.IsEmpty() && s2.IsEmpty()", pTestHandler->m_pContext->VGetExpression().GetData());
}

TEST_F(AssertTest, TestStateInformation)
{
  const shared_ptr<ISpAssertSettings> pAssertSettings(
      MakeStrongPtr<ISpAssertSettings>(cServiceLocator::GetInstance()->GetService<ISpAssertSettings>()));

  cTestHandler* pTestHandler = const_cast<cTestHandler*>(dynamic_cast<const cTestHandler*>(pAssertSettings->VGetHandler().get()));

  int a = 5;
  int b = 6;
  SP_ASSERT(a == b);
  EXPECT_EQ(0, pTestHandler->m_pContext->VGetVariablesArray().size());

  // cppcheck-suppress unreadVariable
  int* p = NULL;
  pTestHandler->Reset();
  SP_ASSERT(p)(p);
  EXPECT_EQ(1, pTestHandler->m_pContext->VGetVariablesArray().size());
  auto iter = pTestHandler->m_pContext->VGetVariablesArray()[0];
  EXPECT_STREQ("p", iter.m_VariableName.GetData());
  EXPECT_STREQ("null", iter.m_VariableValue.GetData());

  pTestHandler->Reset();
  SP_ASSERT(a == b)(a)(b);
  EXPECT_EQ(2, pTestHandler->m_pContext->VGetVariablesArray().size());
  iter = pTestHandler->m_pContext->VGetVariablesArray()[0];
  EXPECT_STREQ("a", iter.m_VariableName.GetData());
  EXPECT_STREQ("5", iter.m_VariableValue.GetData());
  iter = pTestHandler->m_pContext->VGetVariablesArray()[1];
  EXPECT_STREQ("b", iter.m_VariableName.GetData());
  EXPECT_STREQ("6", iter.m_VariableValue.GetData());

  cString s1 = "ankur";
  cString s2 = "abc";

  pTestHandler->Reset();
  SP_ASSERT(s1.IsEmpty() && s2.IsEmpty())(s1)(s2);
  EXPECT_EQ(2, pTestHandler->m_pContext->VGetVariablesArray().size());
  iter = pTestHandler->m_pContext->VGetVariablesArray()[0];
  EXPECT_STREQ("s1", iter.m_VariableName.GetData());
  EXPECT_STREQ("ankur", iter.m_VariableValue.GetData());
  iter = pTestHandler->m_pContext->VGetVariablesArray()[1];
  EXPECT_STREQ("s2", iter.m_VariableName.GetData());
  EXPECT_STREQ("abc", iter.m_VariableValue.GetData());
}

TEST_F(AssertTest, TestUserFriendlyMessage)
{
  const shared_ptr<ISpAssertSettings> pAssertSettings(
      MakeStrongPtr<ISpAssertSettings>(cServiceLocator::GetInstance()->GetService<ISpAssertSettings>()));

  cTestHandler* pTestHandler = const_cast<cTestHandler*>(dynamic_cast<const cTestHandler*>(pAssertSettings->VGetHandler().get()));

  int a = 5;
  int b = 6;
  SP_ASSERT(a == b);
  EXPECT_STREQ("", pTestHandler->m_pContext->VGetCustomMessage().GetData());

  // cppcheck-suppress unreadVariable
  int* p = NULL;
  pTestHandler->Reset();
  SP_ASSERT(p)(p);
  EXPECT_STREQ("", pTestHandler->m_pContext->VGetCustomMessage().GetData());

  pTestHandler->Reset();
  SP_ASSERT(a > b)(a)(b).SetCustomMessage("a should be less than b");
  EXPECT_STREQ("a should be less than b", pTestHandler->m_pContext->VGetCustomMessage().GetData());

  cString s1 = "ankur";
  cString s2 = "abc";

  pTestHandler->Reset();
  SP_ASSERT(s1.IsEmpty() && s2.IsEmpty())(s1)(s2);
  EXPECT_STREQ("", pTestHandler->m_pContext->VGetCustomMessage().GetData());

  pTestHandler->Reset();
  SP_ASSERT(s1 == s2)(s1)(s2).SetCustomMessage("strings not equal");
  EXPECT_STREQ("strings not equal", pTestHandler->m_pContext->VGetCustomMessage().GetData());
}

TEST_F(AssertTest, TestIgnoreAll)
{
  const shared_ptr<ISpAssertSettings> pAssertSettings(
      MakeStrongPtr<ISpAssertSettings>(cServiceLocator::GetInstance()->GetService<ISpAssertSettings>()));

  cTestHandler* pTestHandler = const_cast<cTestHandler*>(dynamic_cast<const cTestHandler*>(pAssertSettings->VGetHandler().get()));

  pTestHandler->m_Action = AssertAction::IgnoreAll;

  int a = 5;
  int b = 6;
  SP_ASSERT(a == b);
  EXPECT_FALSE(pTestHandler->m_pContext == NULL);

  pTestHandler->Reset();
  // cppcheck-suppress unreadVariable
  int* p = NULL;
  SP_ASSERT(p)(p);
  EXPECT_TRUE(pTestHandler->m_pContext == NULL);

  SP_ASSERT(a == b)(a)(b);
  EXPECT_TRUE(pTestHandler->m_pContext == NULL);
}

TEST_F(AssertTest, TestIgnoreForever)
{
  struct Local
  {
    static void f() { SP_ASSERT(false); }
  };

  const shared_ptr<ISpAssertSettings> pAssertSettings(
    MakeStrongPtr<ISpAssertSettings>(cServiceLocator::GetInstance()->GetService<ISpAssertSettings>()));

  cTestHandler* pTestHandler = const_cast<cTestHandler*>(dynamic_cast<const cTestHandler*>(pAssertSettings->VGetHandler().get()));

  pTestHandler->m_Action = AssertAction::IgnoreForever;

  Local::f();
  EXPECT_EQ(SP_LINE - 11, pTestHandler->m_pContext->VGetLineNumber());

  pTestHandler->Reset();
  Local::f();  // should be ignored the second time
  EXPECT_TRUE(pTestHandler->m_pContext == NULL);

  Local::f();  // should be ignored the third time
  EXPECT_TRUE(pTestHandler->m_pContext == NULL);
}

TEST_F(AssertTest, DefaultLogger)
{
  const shared_ptr<ISpAssertSettings> pAssertSettings(
      MakeStrongPtr<ISpAssertSettings>(cServiceLocator::GetInstance()->GetService<ISpAssertSettings>()));

  cTestHandler* pTestHandler = const_cast<cTestHandler*>(dynamic_cast<const cTestHandler*>(pAssertSettings->VGetHandler().get()));

  // cppcheck-suppress unreadVariable
  int* p = NULL;
  SP_ASSERT(p)(p);
  EXPECT_EQ(1, pTestHandler->m_pContext->VGetVariablesArray().size());
  auto iter = pTestHandler->m_pContext->VGetVariablesArray()[0];
  EXPECT_STREQ("p", iter.m_VariableName.GetData());
  EXPECT_STREQ("null", iter.m_VariableValue.GetData());
}

TEST_F(AssertTest, CustomLogger)
{
  const shared_ptr<ISpAssertSettings> pAssertSettings(
      MakeStrongPtr<ISpAssertSettings>(cServiceLocator::GetInstance()->GetService<ISpAssertSettings>()));

  cTestHandler* pTestHandler = const_cast<cTestHandler*>(dynamic_cast<const cTestHandler*>(pAssertSettings->VGetHandler().get()));

  unique_ptr<ISpAssertLogger> pLogger(new cTestLogger());
  pLogger->VInitialize();
  pAssertSettings->VSetLogger(std::move(pLogger));

  // cppcheck-suppress unreadVariable
  int* p = NULL;
  SP_ASSERT(p)(p);
  EXPECT_EQ(1, pTestHandler->m_pContext->VGetVariablesArray().size());
  auto iter = pTestHandler->m_pContext->VGetVariablesArray()[0];
  EXPECT_STREQ("p", iter.m_VariableName.GetData());
  EXPECT_STREQ("null", iter.m_VariableValue.GetData());
}
