//  *******************************************************************************************************************
//  BTCountLimitNode   version:  1.0   Ankur Sheel  date: 2014/12/31
//  *******************************************************************************************************************
//
//  *******************************************************************************************************************
#ifndef __BTCOUNTLIMITNODE_H__
#define __BTCOUNTLIMITNODE_H__

#include "BTDecoratorNode.h"

namespace AI
{
  class cBTCountLimitDecoratorNode : public cBTDecoratorNode
  {
    DEFINE_NODE_PROPERTY(cBTCountLimitDecoratorNode, int, Limit)
    DEFINE_NODE_PROPERTY(cBTCountLimitDecoratorNode, bool, AllowReinitialize)

  public:
    ~cBTCountLimitDecoratorNode();

  private:
    AI_API cBTCountLimitDecoratorNode();
    void VOnInitialize(void* pOwner) OVERRIDE;
    BT_STATUS::Enum VOnUpdate(void* pOwner, float deltaTime) OVERRIDE;
    void VReset() OVERRIDE;

  private:
    int m_Count;

    friend class cBTNodeFactory;
  };
}  // namespace AI
#endif  // __BTCOUNTLIMITNODE_H__
