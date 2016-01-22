#ifndef TYPEREGISTAR_H__
#define TYPEREGISTAR_H__

#include <stddef.h>

//  For custom classes put TypeRegistar specialization at your type definition.
//  Use explicit TypeIDs enum to get unique id for your type

namespace Base
{
  class cString;
}

namespace Base
{
  namespace TypeRegistar
  {
    template<typename T> struct TypeRegistar;
    //  for custom types (to avoid predeclaration here, define type ID in the place of type definition)

    //  for build-in & basic types 
    template<> struct TypeRegistar<int8> { enum { Id = __LINE__ }; };
    template<> struct TypeRegistar<int16> { enum { Id = __LINE__ }; };
    template<> struct TypeRegistar<int32> { enum { Id = __LINE__ }; };
    template<> struct TypeRegistar<int64> { enum { Id = __LINE__ }; };
    template<> struct TypeRegistar<uint8> { enum { Id = __LINE__ }; };
    template<> struct TypeRegistar<uint16> { enum { Id = __LINE__ }; };
    template<> struct TypeRegistar<uint32> { enum { Id = __LINE__ }; };
    template<> struct TypeRegistar<uint64> { enum { Id = __LINE__ }; };
    template<> struct TypeRegistar<real32> { enum { Id = __LINE__ }; };
    template<> struct TypeRegistar<cString> { enum { Id = __LINE__ }; };
    template<> struct TypeRegistar<double> { enum { Id = __LINE__ }; };
    template<> struct TypeRegistar<bool> { enum { Id = __LINE__ }; };
  }
}

#endif  // TYPEREGISTAR_H__
