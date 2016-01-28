//  *******************************************************************************************************************
//  BTProbabilitySelectorNode   version:  1.0   Ankur Sheel  date: 2014/12/19
//  *******************************************************************************************************************
//
//  *******************************************************************************************************************
#ifndef __BTPROBABILITYSELECTORNODE_H__
#define __BTPROBABILITYSELECTORNODE_H__

#include "AIDefines.h"
#include "BTBaseSelectorNode.h"

namespace Utilities
{
  class IRandomGenerator;
}

namespace AI
{
  class cBTNodeFactory;
}

namespace AI
{
  class cBTProbabilitySelectorNode : public cBTBaseSelectorNode
  {
  public:
    ~cBTProbabilitySelectorNode();
    AI_API shared_ptr<cBTProbabilitySelectorNode> SetWeight(const int weight);
    const std::vector<int>& GetWeights() const { return m_Weights; }
  private:
    AI_API cBTProbabilitySelectorNode();
    void VOnInitialize(void* pOwner) OVERRIDE;
    void VCalculateNextChildIndex() OVERRIDE;
    bool VIsDone() OVERRIDE { return m_RemainingIndices.empty(); }
  private:
    int m_Total;
    std::vector<int> m_Weights;
    std::vector<int> m_RemainingIndices;

    friend class cBTNodeFactory;
  };
}  // namespace AI
#endif  // __BTPROBABILITYSELECTORNODE_H__
