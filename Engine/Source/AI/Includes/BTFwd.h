//  *******************************************************************************************************************
//  BTFwd   version:  1.0   Ankur Sheel  date: 2014/12/09
//  *******************************************************************************************************************
//
//  *******************************************************************************************************************
#ifndef __BTFWD_H__
#define __BTFWD_H__

namespace AI
{
  namespace BT_STATUS
  {
    enum Enum
    {
      Invalid,
      Running,
      Success,
      Failure,
      Aborted
    };
  }  // namespace BT_STATUS

  namespace BT_POLICY
  {
    enum Enum
    {
      RequireOne,
      RequireAll
    };
  }

  class cBTBehaviorNode;
  class cBTBehaviorTree;

  typedef shared_ptr<cBTBehaviorNode> BTNodeStrongPtr;
  typedef weak_ptr<cBTBehaviorNode> BTNodeWeakPtr;
  typedef std::vector<BTNodeStrongPtr> BTNodes;
  typedef shared_ptr<cBTBehaviorTree> BTTreeStrongPtr;
  typedef weak_ptr<cBTBehaviorTree> BTTreeWeakPtr;

#define DEFINE_NODE_PROPERTY(CLASS, TYPE, NAME)                                                                        \
  \
public:                                                                                                                \
  shared_ptr<CLASS> const Set##NAME(const TYPE& _##NAME)                                                               \
  {                                                                                                                    \
    m_##NAME = _##NAME;                                                                                                \
    return dynamic_pointer_cast<CLASS>(shared_from_this());                                                            \
  }                                                                                                                    \
  const TYPE& Get##NAME() const { return m_##NAME; }                                                                   \
  \
private:                                                                                                               \
  TYPE m_##NAME;
}  // namespace AI
#endif  // __BTFWD_H__
