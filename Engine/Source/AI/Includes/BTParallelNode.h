//  *******************************************************************************************************************
//  BTParallelNode   version:  1.0   Ankur Sheel  date: 2014/12/10
//  *******************************************************************************************************************
//
//  *******************************************************************************************************************
#ifndef __BTPARALLELNODE_H__
#define __BTPARALLELNODE_H__
#include "AIDefines.h"
#include "BTCompositeNode.h"

namespace AI
{
  class cBTParallelNode : public cBTCompositeNode
  {
    DEFINE_NODE_PROPERTY(cBTParallelNode, BT_POLICY::Enum, SuccessPolicy)
    DEFINE_NODE_PROPERTY(cBTParallelNode, BT_POLICY::Enum, FailurePolicy)

  public:
    AI_API cBTParallelNode();
    ~cBTParallelNode();

  private:
    void VOnInitialize(void* pOwner) OVERRIDE;
    BT_STATUS::Enum VOnUpdate(void* pOwner, float deltaTime) OVERRIDE;
    void VOnTerminate(const BT_STATUS::Enum status) OVERRIDE;

  private:
    std::vector<BT_STATUS::Enum> m_ChildrenStatus;
  };
}  // namespace AI
#endif  // __BTPARALLELNODE_H__
