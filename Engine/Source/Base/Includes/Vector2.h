//  *******************************************************************************************************************
//  Vector2D   version:  1.0   Ankur Sheel  date: 2012/08/28
//  *******************************************************************************************************************
//
//  *******************************************************************************************************************
#ifndef Vector2_h__
#define Vector2_h__

#include "BaseDefines.h"
#include "MathIncludes.h"

namespace Base
{
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /// @brief Encapsulates a 2D vector with X and Y components .
  ///
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  class cVector2
  {
  public:
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Constructor
    ///
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    BASE_API cVector2();
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Constructor
    ///
    /// @param[in] X The X component of the vector
    /// @param[in] Y The Y component of the vector
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    BASE_API cVector2(const float X, const float Y);
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Constructor
    ///
    /// @param[in] arr Array of float for the /code v component
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    explicit BASE_API cVector2(float arr[]);
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Constructor
    ///
    /// @param[in] pt Point structure to convert to cVector2
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    explicit BASE_API cVector2(POINT pt);
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Returns true if the vector is 0
    ///
    /// @return True if the vector is 0
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    bool IsZero() const { return LengthSquared() < EpsilonFloat; }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Get the perpendicular vector about the X axis.
    ///
    /// @return perpendicular vector about the X axis.
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    cVector2 PerpendicularAboutXAxis() const { return cVector2(y, -x); }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Get the perpendicular vector about the Y axis.
    ///
    /// @return perpendicular vector about the Y axis.
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    cVector2 PerpendicularAboutYAxis() const { return cVector2(-y, x); }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Gets the reverse vector
    ///
    /// @return Reverse of this vector
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    cVector2 GetReverse() const { return cVector2(-x, -y); }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Checks if the passed in vector is equal to the current vector
    ///
    /// @param[in] inVec The vector to check against.
    /// @return True if the 2 vectors are equal. False otherwise.
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    bool operator==(const cVector2& inVec) const { return (isEqual(x, inVec.x) && isEqual(y, inVec.y)); }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Checks if the passed in vector is not equal to the current vector
    ///
    /// @param[in] inVec The vector to check against.
    /// @return True if the 2 vectors are not equal. False otherwise.
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    bool operator!=(const cVector2& inVec) const { return (!isEqual(x, inVec.x) || !isEqual(y, inVec.y)); }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Gets the length of the vector
    ///
    /// @return Length of the vector
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    BASE_API float Length() const;
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Gets the squared length of the vector
    ///
    /// @return Squared length of the vector
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    BASE_API float LengthSquared() const;
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Converts the Vector into a unit vector
    ///
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    BASE_API void Normalize();
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Calculates and returns the dot product
    ///
    /// @param[in] inVec The vector with which the dot product needs to be calculated
    /// @return The dot product between this vector and the one passed as a parameter
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    BASE_API float Dot(const cVector2& inVec) const;
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Calculates and returns the cross product
    ///
    /// @param[in] inVec The vector with which the cross product needs to be calculated
    /// @return The dot product between this vector and the one passed as a parameter.
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    BASE_API float Cross(const cVector2& inVec) const;
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Gets the distance between this and the passed in vector
    ///
    /// @param[in] inVec The vector with which the distance needs to be calculated
    /// @return Euclidean distance
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    BASE_API float Distance(const cVector2& inVec) const;
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Gets the squared distance between this and the passed in vector
    ///
    /// @param[in] inVec The vector with which the squared distance needs to be calculated
    /// @return Squared Euclidean distance
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    BASE_API float DistanceSquared(const cVector2& inVec) const;
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Reflects the vector around the passed in axis
    ///
    /// @param[in] axis The vector that should be reflected
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    BASE_API void Reflect(cVector2 axis);
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Sets X and Y to their absolute values
    ///
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    BASE_API void AbsTo();
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Get the largest component and return a signed, unit vector containing only that component
    ///
    /// @return A signed, unit vector containing only the largest component
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    BASE_API cVector2 MajorAxis() const;
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Get the smallest component and return a signed, unit vector containing only that component
    ///
    /// @return A signed, unit vector containing only the smallest component
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    BASE_API cVector2 MinorAxis() const;
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Changes the sign of the components
    ///
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    BASE_API void NegTo();
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Adds the passed in vector to the current vector.
    ///
    /// @param[in] inVec The vector that is to be added
    /// @return The vector after the addition
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    BASE_API const cVector2& operator+=(const cVector2& inVec);
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Subtracts the passed in vector from the current vector.
    ///
    /// @param[in] inVec The vector that is to be subtracted
    /// @return The vector after the subtraction
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    BASE_API const cVector2& operator-=(const cVector2& inVec);
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Multiplies the passed in vector to the current vector.
    ///
    /// @param[in] inVec The vector that is to be multiplied.
    /// @return The vector after the multiplication
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    BASE_API const cVector2& operator*=(const cVector2& inVec);
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Multiplies the passed in value to each component of the current vector.
    ///
    /// @param[in] Val The scalar value that is to be multiplied.
    /// @return The vector after the multiplication
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    BASE_API const cVector2& operator*=(const float Val);
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Divides the passed in value from each component of the current vector.
    ///
    /// @param[in] Val The scalar value that is to be divided.
    /// @return The vector after the multiplication
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    BASE_API const cVector2& operator/=(const float Val);
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Returns the component at the index
    ///
    /// @param[in] index The index to get
    /// @return The value at the index
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    BASE_API float& operator[](const unsigned int index);
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Returns the component at the index
    ///
    /// @param[in] index The index to get
    /// @return The value at the index
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    BASE_API const float operator[](const unsigned int index) const;
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Makes a normalized vector which is at a given angle from the X axis
    ///
    /// @param[in] AngleInDegrees The angle (in degrees) at which the vector should be.
    /// @return The normalized vector at a the given angle
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    BASE_API static cVector2 MakeVectorFromAngle(const float AngleInDegrees);
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Returns a vector between start and end which /code lerpfactor away from start.
    ///
    /// @param[in] start The start vector
    /// @param[in] end The end vector
    /// @param[in] lerpFactor The percentage through which the new vector should be away from start
    /// @return Put The vector which is lerpfactor away from start
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    BASE_API static cVector2 Lerp(const cVector2& start, const cVector2& end, float lerpFactor);

    BASE_API static cVector2 Rotate(const cVector2& vec, const float angleInRad);
    friend BASE_API std::ostream& operator<<(std::ostream& out, const cVector2& str);

  public:
    union
    {
      struct
      {
        float x;  ///< X component of Vector2
        float y;  ///< Y component of Vector2
      };
      float v[2];
    };

    BASE_API static const cVector2 Zero;
    BASE_API static const cVector2 One;
  };

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /// Returns a vector which is the result of invec2 + invec1
  ///
  /// @param[in] inVec1 The first vector that is to be added
  /// @param[in] inVec2 The second vector that is to be added
  /// @return The vector after the addition
  ///
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  inline cVector2 operator+(const cVector2& inVec1, const cVector2& inVec2);
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /// Returns a vector which is the result of invec2 - invec1
  ///
  /// @param[in] inVec1 The first vector
  /// @param[in] inVec2 The vector that is to be subtracted
  /// @return The vector after the Subtraction
  ///
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  inline cVector2 operator-(const cVector2& inVec1, const cVector2& inVec2);
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /// Returns a vector which is the result of Multiplication of the passed in vector with the scalar value
  ///
  /// @param[in] inVec1 The vector
  /// @param[in] Val The scalar value that is to be multiplied.
  /// @return The vector after the multiplication
  ///
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  inline cVector2 operator*(const cVector2& inVec1, const float Val);
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /// Returns a vector which is the result of Multiplication of of each component of the passed in vector with the
  /// scalar value
  ///
  /// @param[in] Val The scalar value that is to be multiplied.
  /// @param[in] inVec1 The vector
  /// @return The vector after the multiplication
  ///
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  inline cVector2 operator*(const float Val, const cVector2& inVec1);
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /// Returns a vector which is the result of Multiplication of of each component of the passed in vector with the
  /// scalar value
  ///
  /// @param[in] inVec1 The first vector that is to be multiplied
  /// @param[in] inVec2 The second vector that is to be multiplied
  /// @return The vector after the multiplication
  ///
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  inline cVector2 operator*(const cVector2& inVec1, const cVector2& inVec2);
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /// Returns a vector which is the result of multiplication of the passed in vectors
  ///
  /// @param[in] inVec1 The vector
  /// @param[in] Val The scalar value that is to be divided.
  /// @return The vector after the multiplication
  ///
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  inline cVector2 operator/(const cVector2& inVec1, const float Val);
  BASE_API std::ostream& operator<<(std::ostream& out, const cVector2& v);

#include "Vector2.inl"
}  // namespace Base

#endif  // Vector2_h__
