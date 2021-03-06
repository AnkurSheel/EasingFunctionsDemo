//  *******************************************************************************************************************
//  EventData version:  1.0   Ankur Sheel  date: 2013/07/25
//  *******************************************************************************************************************
//  purpose:
//  *******************************************************************************************************************
#ifndef EventData_hxx__
#define EventData_hxx__

namespace GameBase
{
  typedef Base::cHashedString EventType;

  class IEventData
  {
  public:
    virtual ~IEventData() {}
    virtual UINT64 VGetEventID() const = 0;
    virtual Base::cString VGetEventName() const = 0;
  };
}  // namespace GameBase
#endif  // EventData_hxx__
