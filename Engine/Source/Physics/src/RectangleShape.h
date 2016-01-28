//  *******************************************************************************************************************
//  RectangleShape version:  1.0   Ankur Sheel  date: 2013/06/07
//  *******************************************************************************************************************
//  purpose:
//  *******************************************************************************************************************

#ifndef RectangleShape_hxx
#define RectangleShape_hxx

#include "Shape.h"

namespace Physics
{
  class cRectangleShape : public cShape
  {
  public:
    cRectangleShape();
    ~cRectangleShape();
    void VInitialize(const Base::cVector3& minBound, const Base::cVector3& maxBound) OVERRIDE;
    IShape* VDuplicate() OVERRIDE;
    const Base::cHashedString& VGetShapeName() const OVERRIDE { return m_Name; }
  public:
    static Base::cHashedString m_Name;  ///< The shape name
  };
}  // namespace Physics
#endif  // RectangleShape_hxx
