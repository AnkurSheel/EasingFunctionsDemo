#include "stdafx.h"
#include "VertexStruct.h"

using namespace Graphics;
using namespace Base;

//  *******************************************************************************************************************
stTexVertex::stTexVertex()
	: m_fX(0.0f)
	, m_fY(0.0f)
	, m_fZ(0.0f)
	, m_fTex0(0.0f)
	, m_fTex1(0.0f)
{
}

//  *******************************************************************************************************************
stTexVertex::stTexVertex(const float fX, const float fY, const float fZ, const float fTex0, const float fTex1 )
	: m_fX(fX)
	, m_fY(fY)
	, m_fZ(fZ)
	, m_fTex0(fTex0)
	, m_fTex1(fTex1)
{
}
