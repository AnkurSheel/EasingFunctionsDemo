//  *******************************************************************************************************************
//  CircleShape   version:  1.0   Ankur Sheel  date: 2015/05/05
//  *******************************************************************************************************************
//
//  *******************************************************************************************************************
#ifndef CIRCLESHAPE_H__
#define CIRCLESHAPE_H__

#include "Shape.h"

namespace Physics
{
  class cCircleShape : public cShape
  {
  public:
    cCircleShape();
    ~cCircleShape();
    void VInitialize(const Base::cVector3& minBound, const Base::cVector3& maxBound) OVERRIDE;
    IShape* VDuplicate() OVERRIDE;
    const Base::cHashedString& VGetShapeName() const OVERRIDE { return m_Name; }
  public:
    static Base::cHashedString m_Name;  ///< The shape name
  };
}  // namespace Physics
#endif  // CIRCLESHAPE_H__
