//  *******************************************************************************************************************
//  ServiceLocator   version:  1.0   Ankur Sheel  date: 2014/11/17
//  *******************************************************************************************************************
//
//  *******************************************************************************************************************
#ifndef __SERVICELOCATOR_H__
#define __SERVICELOCATOR_H__

#include "BaseDefines.h"
#include "Singleton.h"

namespace Base
{
  class cServiceLocator
  {
    DECLARE_SINGLETON(cServiceLocator, BASE_API)
    typedef std::map<cString, shared_ptr<void> > ServiceMap;

  public:
    BASE_API ~cServiceLocator();

    template <class T>
    void Register(const shared_ptr<T> pObject);

    template <class T>
    weak_ptr<T> GetService();

    template <class T>
    bool IsServiceRegistered();

  private:
    cServiceLocator();

  private:
    ServiceMap m_RegisteredTypes;
  };

  //  ***************************************************************************************************************
  template <class T>
  void cServiceLocator::Register(const shared_ptr<T> pObject)
  {
    cString name = typeid(T).name();
    if (!IsServiceRegistered<T>())
    {
      m_RegisteredTypes[name] = static_pointer_cast<void>(pObject);
    }
    else
    {
      SP_ASSERT(false)(name).SetCustomMessage("Service is already registered");
    }
  }

  //  ***************************************************************************************************************
  template <class T>
  weak_ptr<T> cServiceLocator::GetService()
  {
    cString name = typeid(T).name();
    if (IsServiceRegistered<T>())
    {
      return static_pointer_cast<T>(m_RegisteredTypes[typeid(T).name()]);
    }
    return std::weak_ptr<T>();
  }

  //  ***************************************************************************************************************
  template <class T>
  bool cServiceLocator::IsServiceRegistered()
  {
    cString name = typeid(T).name();
    auto findIt = m_RegisteredTypes.find(name);
    return (findIt != m_RegisteredTypes.end());
  }
}  // namespace Base
#endif  // __SERVICELOCATOR_H__
