//  *******************************************************************************************************************
//  Any   version:  1.0   Ankur Sheel  date: 2015/10/31
//  *******************************************************************************************************************
//
//  *******************************************************************************************************************
#ifndef ANY_H__
#define ANY_H__

#include "TypeRegistar.h"

namespace Base
{
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /// @brief boost::any without RTTI
  ///
  ///
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  class Any
  {
  private:
    struct DataHolderBase;

    template <typename T>
    struct DataHolder;

  public:
    BASE_API Any();
    BASE_API Any(const Any& other);
    BASE_API Any& operator=(const Any& other);

    template <typename T>
    Any(const T& val);

    BASE_API ~Any();

    bool operator!() const { return m_data != nullptr; }
    bool IsValid() const { return (m_data != nullptr); }
    BASE_API void Clear();
    BASE_API bool Compare(const Any& other) const;

    template <typename T>
    void Set(const T& value);

    template <typename T>
    T* Get() const;

    template <typename T>
    bool Is() const;

  private:
    DataHolderBase* m_data;
    int m_typeID;
  };

  struct Any::DataHolderBase
  {
    virtual DataHolderBase* Clone() const = 0;
    virtual ~DataHolderBase() {}
  };

  template <typename T>
  struct Any::DataHolder : Any::DataHolderBase
  {
    DataHolder(const T& data)
      : data(data)
    {
    }

    T data;

    virtual DataHolderBase* Clone() const { return DEBUG_NEW(DataHolder<T>)(*this); }
  };

  template <typename T>
  Any::Any(const T& val)
    : m_data(NULL)
    , m_typeID(-1)
  {
    Set(val);
  }

  template <typename T>
  void Any::Set(const T& value)
  {
    Clear();

    //  GOT a compile error here? error C2065: 'Id' : undeclared identifier
    //
    //  you need to register your type, example
    //  template<> struct TypeRegistar::TypeRegistar<GameType> { enum { Id = TYPE_ID_GAME_TYPE}; };
    //
    m_data = DEBUG_NEW(DataHolder<T>)(value);
    m_typeID = TypeRegistar::TypeRegistar<T>::Id;
  }

  template <typename T>
  T* Any::Get() const
  {
    if (m_typeID == TypeRegistar::TypeRegistar<T>::Id)
    {
      return &static_cast<DataHolder<T>*>(m_data)->data;
    }

    return nullptr;
  }

  template <typename T>
  bool Any::Is() const
  {
    return m_typeID == TypeRegistar::TypeRegistar<T>::Id;
  }
}
#endif  // ANY_H__
