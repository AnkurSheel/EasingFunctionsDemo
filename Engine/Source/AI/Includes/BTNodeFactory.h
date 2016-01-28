//  *******************************************************************************************************************
//  BTNodeFactory   version:  1.0   Ankur Sheel  date: 2014/12/11
//  *******************************************************************************************************************
//
//  *******************************************************************************************************************
#ifndef __BTNODEFACTORY_H__
#define __BTNODEFACTORY_H__

namespace AI
{
  class cBTBehaviorNode;
}

namespace AI
{
  class cBTNodeFactory
  {
  public:
    template <class NODE>
    static shared_ptr<NODE> Create()
    {
      return shared_ptr<NODE>(DEBUG_NEW NODE());
    }

  private:
    cBTNodeFactory() {}
    ~cBTNodeFactory() {}
  };
}  // namespace AI
#endif  // __BTNODEFACTORY_H__
