#include "stdafx.h"
#include "Any.h"
#include "myString.h"

using namespace Base;

//  *******************************************************************************************************************
Any::Any()
  : m_data(NULL)
  , m_typeID(-1)
{
}

//  *******************************************************************************************************************
Any::Any(const Any& other)
{
  m_typeID = other.m_typeID;

  m_data = NULL;
  if (other.m_data)
  {
    m_data = other.m_data->Clone();
  }
}

//  *******************************************************************************************************************
Any::~Any()
{
  Clear();
}

//  *******************************************************************************************************************
Any& Any::operator=(const Any& other)
{
  Clear();

  m_typeID = other.m_typeID;

  m_data = NULL;
  if (other.m_data)
  {
    m_data = other.m_data->Clone();
  }

  return *this;
}

//  *******************************************************************************************************************
void Any::Clear()
{
  SafeDelete(&m_data);

  m_data = NULL;
  m_typeID = -1;
}

//  *******************************************************************************************************************
bool Any::Compare(const Any& other) const
{
  if (m_typeID != other.m_typeID)
  {
    return false;
  }

  // type specific comparison checks.
  switch (m_typeID)
  {
    case TypeRegistar::TypeRegistar<int8>::Id:
    {
      return *Get<int8>() == *other.Get<int8>();
      break;
    }

    case TypeRegistar::TypeRegistar<int16>::Id:
    {
      return *Get<int16>() == *other.Get<int16>();
      break;
    }

    case TypeRegistar::TypeRegistar<int32>::Id:
    {
      return *Get<int32>() == *other.Get<int32>();
      break;
    }

    case TypeRegistar::TypeRegistar<int64>::Id:
    {
      return *Get<int64>() == *other.Get<int64>();
      break;
    }

    case TypeRegistar::TypeRegistar<uint8>::Id:
    {
      return *Get<uint8>() == *other.Get<uint8>();
      break;
    }

    case TypeRegistar::TypeRegistar<uint16>::Id:
    {
      return *Get<uint16>() == *other.Get<uint16>();
      break;
    }

    case TypeRegistar::TypeRegistar<uint32>::Id:
    {
      return *Get<uint32>() == *other.Get<uint32>();
      break;
    }

    case TypeRegistar::TypeRegistar<uint64>::Id:
    {
      return *Get<uint64>() == *other.Get<uint64>();
      break;
    }

    case TypeRegistar::TypeRegistar<real32>::Id:
    {
      return *Get<real32>() == *other.Get<real32>();
      break;
    }

    case TypeRegistar::TypeRegistar<cString>::Id:
    {
      return *Get<cString>() == *other.Get<cString>();
      break;
    }

    case TypeRegistar::TypeRegistar<double>::Id:
    {
      return *Get<double>() == *other.Get<double>();
      break;
    }

    case TypeRegistar::TypeRegistar<bool>::Id:
    {
      return *Get<bool>() == *other.Get<bool>();
      break;
    }

    default:
    {
      SP_ASSERT_ERROR(false)(m_typeID).SetCustomMessage("Could not find comparison for TypeId ");
      break;
    }
  }

  return false;
}
