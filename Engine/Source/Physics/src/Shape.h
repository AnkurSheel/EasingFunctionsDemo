//  *******************************************************************************************************************
//  Shape version:  1.0   Ankur Sheel  date: 2013/06/22
//  *******************************************************************************************************************
//  purpose:
//  *******************************************************************************************************************

#ifndef Shape_h
#define Shape_h

#include "Shape.hxx"
#include "vector3.h"

namespace Physics
{
	class cShape
		: public IShape
	{
	public:
		cShape();
		virtual ~cShape();
		explicit cShape(const cShape & other);
		cShape & operator =(const cShape & other);
		virtual const Base::cHashedString& VGetShapeName() const = 0;
		const Base::cVector3& VGetCenter() const OVERRIDE { return m_Center; }
		const Base::cVector3& VGetHalfExtents() const OVERRIDE { return m_HalfExtents; }
		Base::cVector3 VGetMinBound() const OVERRIDE { return m_Center - m_HalfExtents; }
		Base::cVector3 VGetMaxBound() const OVERRIDE { return m_Center + m_HalfExtents; }
		float VGetRadius() const OVERRIDE { return Base::Max<float>(m_HalfExtents.x, m_HalfExtents.y); }
		void VScale(const float scaleFactor) OVERRIDE { m_HalfExtents *= scaleFactor; }

		const Base::cHashedString& VGetName() const OVERRIDE { return VGetShapeName(); }
		void VOnMoved(const Base::cVector3 & deltaPosition);
		void VUpdateBounds(const Base::cVector3 & position, const Base::cVector3 & minBound, const Base::cVector3 & maxBound) override;


	protected:
		Base::cVector3	m_Center;
		Base::cVector3	m_HalfExtents;
	};
}  // namespace Physics
#endif  // Shape_h
