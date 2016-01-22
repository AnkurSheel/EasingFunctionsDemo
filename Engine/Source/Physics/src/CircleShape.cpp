#include "stdafx.h"
#include "CircleShape.h"

using namespace Physics;
using namespace Base;

cHashedString cCircleShape::m_Name = cHashedString("circle");

//  *******************************************************************************************************************
cCircleShape::cCircleShape()
{
}

//  *******************************************************************************************************************
cCircleShape::~cCircleShape()
{
}

//  *******************************************************************************************************************
IShape * cCircleShape::VDuplicate()
{
	cCircleShape * pShape = DEBUG_NEW cCircleShape(*this);
	return pShape;
}

//  *******************************************************************************************************************
void cCircleShape::VInitialize(const cVector3 & minBound, const cVector3 & maxBound)
{
	VUpdateBounds(cVector3::Zero, minBound, maxBound);
}

//  *******************************************************************************************************************
IShape * IShape::CreateCircleShape()
{
	return DEBUG_NEW cCircleShape();
}
