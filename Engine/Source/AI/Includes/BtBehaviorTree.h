//  *******************************************************************************************************************
//  BtBehaviorTree   version:  1.0   Ankur Sheel  date: 2015/05/12
//  *******************************************************************************************************************
//
//  *******************************************************************************************************************
#ifndef BTBEHAVIORTREE_H__
#define BTBEHAVIORTREE_H__

#include "AIDefines.h"
#include "BTFwd.h"

namespace AI
{
  class cBTBehaviorTree
  {
  public:
    AI_API cBTBehaviorTree(BTNodeStrongPtr pRoot);
    AI_API ~cBTBehaviorTree();
    AI_API BT_STATUS::Enum Tick(void* pOwner, float deltaTime);
    void SetRoot(BTNodeStrongPtr pRoot) { m_pRoot = pRoot; }
  private:
    BTNodeStrongPtr m_pRoot;
  };
}  // namespace AI
#endif  // BTBEHAVIORTREE_H__
