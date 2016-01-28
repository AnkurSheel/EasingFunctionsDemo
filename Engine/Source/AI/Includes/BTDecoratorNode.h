//  *******************************************************************************************************************
//  BTDecoratorNode   version:  1.0   Ankur Sheel  date: 2014/12/09
//  *******************************************************************************************************************
//
//  *******************************************************************************************************************
#ifndef __BTDECORATORNODE_H__
#define __BTDECORATORNODE_H__

#include "BTBehaviorNode.h"

namespace AI
{
  class cBTDecoratorNode : public cBTBehaviorNode
  {
  public:
    ~cBTDecoratorNode();
    AI::BTNodeStrongPtr GetChild() const { return m_pChild; }
    template <class PARENT>
    shared_ptr<PARENT> SetChild(BTNodeWeakPtr pChild);

  protected:
    AI_API cBTDecoratorNode();
    void VOnInitialize(void* pOwner) OVERRIDE;
    BT_STATUS::Enum VOnUpdate(void* pOwner, float deltaTime) OVERRIDE;
    void VReset() OVERRIDE;

  protected:
    BTNodeStrongPtr m_pChild;
  };

  //  ********************************************************************************************************************
  template <class PARENT>
  shared_ptr<PARENT> cBTDecoratorNode::SetChild(BTNodeWeakPtr pChild)
  {
    SP_ASSERT(m_pChild == NULL).SetCustomMessage("Decorator Node can have only 1 child");
    BTNodeStrongPtr pThisPtr = shared_from_this();
    if (m_pChild == NULL)
    {
      m_pChild = MakeStrongPtr(pChild);
      m_pChild->SetParent(pThisPtr);
    }
    return dynamic_pointer_cast<PARENT>(pThisPtr);
  }

}  // namespace AI
#endif  // __BTDECORATORNODE_H__
