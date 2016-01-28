//  *******************************************************************************************************************
//  BTNode   version:  1.0   Ankur Sheel  date: 2014/12/09
//  *******************************************************************************************************************
//
//  *******************************************************************************************************************
#ifndef __BTNODE_H__
#define __BTNODE_H__

#include "AIDefines.h"
#include "BTFwd.h"

namespace AI
{
  class cBTBehaviorNode : public std::enable_shared_from_this<cBTBehaviorNode>
  {
  public:
    AI_API virtual ~cBTBehaviorNode();
    AI_API virtual void VOnInitialize(void* pOwner);
    AI_API virtual void VReset();

    BT_STATUS::Enum Tick(void* pOwner, float deltaTime);
    void Abort();

    BTNodeWeakPtr GetParent() const { return m_pParent; }
    void SetParent(BTNodeStrongPtr pParent) { m_pParent = pParent; }
    bool IsRunning() { return m_ReturnStatus == BT_STATUS::Running; }
    template <class ANCESTOR>
    shared_ptr<ANCESTOR> FindAncestor();

  protected:
    AI_API cBTBehaviorNode();
    virtual BT_STATUS::Enum VOnUpdate(void* pOwner, float deltaTime) { return BT_STATUS::Invalid; }
    AI_API virtual void VOnTerminate(const BT_STATUS::Enum status);

  protected:
    BTNodeWeakPtr m_pParent;
    BT_STATUS::Enum m_ReturnStatus;
    BT_STATUS::Enum m_TerminateStatus;
    bool m_Initialized;

    friend class cBTNodeFactory;
  };

  //  ********************************************************************************************************************
  template <class ANCESTOR>
  shared_ptr<ANCESTOR> cBTBehaviorNode::FindAncestor()
  {
    BTNodeStrongPtr pNode = MakeStrongPtr(m_pParent);
    if (pNode != NULL)
    {
      shared_ptr<T> pAncestor = NULL;
      while (pNode != NULL && pAncestor == NULL)
      {
        pAncestor = dynamic_pointer_cast<T>(pNode);
        pNode = pNode->getParent();
      }
      return pAncestor;
    }
  }
}  // namespace AI
#endif  // __BTNODE_H__
