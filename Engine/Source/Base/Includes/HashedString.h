//  *******************************************************************************************************************
//  HashedString   version:  1.0   Ankur Sheel  date: 2012/12/26
//  *******************************************************************************************************************
//
//  *******************************************************************************************************************
#ifndef HashedString_h__
#define HashedString_h__

#include <memory>
#include "BaseDefines.h"
#include "myString.h"

namespace Base
{
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /// @brief Stores a string with its Adler-32 checksum value.
  ///
  ///
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  class cHashedString
  {
  public:
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Default Constructor
    ///
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    BASE_API cHashedString();
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Constructor which takes in string as a parameter and stores the Adler-32 checksum for it.
    ///
    /// @param[in] str The string from which the hash has to be created
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    BASE_API explicit cHashedString(const cString& str);
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Copy Constructor
    ///
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    BASE_API cHashedString(const cHashedString& other);
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Assignment Operator
    ///
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    BASE_API cHashedString& operator=(const cHashedString& other);
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Returns the hash
    ///
    /// @returns The calculated hash
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    BASE_API UINT64 GetHash() const { return m_Checksum & 0xffffffffL; }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Returns the string
    ///
    /// @returns The string
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    BASE_API const Base::cString& GetString() const { return m_String; }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Returns true if the checksum of other is equal to the checksum of this hashed string
    ///
    /// @return True if the checksums are equal. False otherwise
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    BASE_API bool operator==(cHashedString const& other) const;
    BASE_API bool operator!=(cHashedString const& other) const;
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Calculates the adler-32 checksum for str
    ///
    /// @param[in] str The string from which the checksum has to be created
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    BASE_API static uint64 CalculateChecksum(const cString& str);

  private:
    UINT64 m_Checksum;  ///< The Adler-32 checksum associated with this string.
    cString m_String;   ///< The string
  };
}  // namespace Base
#endif  // HashedString_h__
