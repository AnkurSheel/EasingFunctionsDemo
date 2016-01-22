#include "stdafx.h"
#include "RectangleShape.h"

using namespace Physics;
using namespace Base;

cHashedString	cRectangleShape::m_Name = cHashedString("rectangle");

//  *******************************************************************************************************************
cRectangleShape::cRectangleShape()
{
}

//  *******************************************************************************************************************
cRectangleShape::~cRectangleShape()
{
}

//  *******************************************************************************************************************
IShape * cRectangleShape::VDuplicate()
{
	cRectangleShape * pShape = DEBUG_NEW cRectangleShape(*this);
	return pShape;
}

//  *******************************************************************************************************************
void cRectangleShape::VInitialize(const cVector3 & minBound, const cVector3 & maxBound)
{
	VUpdateBounds(cVector3::Zero, minBound, maxBound);
}

//  *******************************************************************************************************************
IShape * IShape::CreateRectangleShape()
{
	return DEBUG_NEW cRectangleShape();
}
