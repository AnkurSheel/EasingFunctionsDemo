#include "stdafx.h"
#include "HScrollBar.h"
#include "Sprite.hxx"

using namespace Graphics;
using namespace Utilities;
using namespace Base;

cHashedString cHScrollBar::m_Name = cHashedString("horizontalscrollbar");

//  *******************************************************************************************************************
cHScrollBar::cHScrollBar()
{
}

//  *******************************************************************************************************************
cHScrollBar::~cHScrollBar()
{
}

//  *******************************************************************************************************************
shared_ptr<IBaseControl> cHScrollBar::VDuplicate()
{
	return std::shared_ptr<IBaseControl>(DEBUG_NEW cHScrollBar(*this));
}

//  *******************************************************************************************************************
bool cHScrollBar::VOnMouseMove(const int X, const int Y)
{
	if (m_DragMode)
	{
		int thumbPos = m_ThumbPos;
		// get the position of the mouse in the scrollbar
		float posRelativeToScrollbar = m_pBtnDecrementArrow->VGetSize().x + X - (m_vControlAbsolutePosition.x + m_pBtnIncrementArrow->VGetSize().x);

		// check if the mouse position is still within the scrollbar
		if (posRelativeToScrollbar >= m_pBtnIncrementArrow->VGetSize().x && posRelativeToScrollbar < (VGetSize().x - m_pBtnDecrementArrow->VGetSize().x))
		{
			// get the area in which the thumb can move
			float thumbRange = (VGetSize().x - m_pBtnIncrementArrow->VGetSize().x) - m_pBtnDecrementArrow->VGetSize().x;

			float pixelsMovedPerIncrement = thumbRange / m_NoOfIncrements;

			// get the cuurent pos of the thum
			for(int counter = 0; counter < m_NoOfIncrements; counter++)
			{
				// get the current pos for this counter
				float posForCounter = m_pBtnDecrementArrow->VGetSize().x + (pixelsMovedPerIncrement * counter);
				if (((posRelativeToScrollbar >= posForCounter)) && (posRelativeToScrollbar <= (posForCounter + pixelsMovedPerIncrement)))
				{
					thumbPos = counter;
					break;
				}
			}
		}
		VSetThumbPosition(thumbPos);
		return true;
	}
	return  cBaseControl::VOnMouseMove(X, Y);
}

//  *******************************************************************************************************************
void cHScrollBar::VSetAbsolutePosition()
{
	cScrollBarControl::VSetAbsolutePosition();
	cVector2 pos = m_vControlAbsolutePosition;
	if (m_pBtnDecrementArrow)
	{
		pos.x += m_MinPos;
		m_pBtnDecrementArrow->VSetPosition(pos);
		if (m_pBGSprite)
		{
			pos.x += m_pBtnDecrementArrow->VGetSize().x;
			m_pBGSprite->VSetPosition(pos);
		}
	}
	if (m_pBtnIncrementArrow)
	{
		pos.x = m_vControlAbsolutePosition.x + VGetSize().x- m_pBtnIncrementArrow->VGetSize().x;
		m_pBtnIncrementArrow->VSetPosition(pos);
	}
	VSetThumbPosition(m_ThumbPos);
}

//  *******************************************************************************************************************
void cHScrollBar::VSetSize(const cVector2 & vSize)
{
	cBaseControl::VSetSize(vSize);
	float backGroundWidth = VGetSize().x;
	float height = VGetSize().y;
	if (m_pBtnDecrementArrow)
	{
		m_pBtnDecrementArrow->VSetSize(cVector2(height, height));
		backGroundWidth -= height;
	}
	if (m_pBtnIncrementArrow)
	{
		m_pBtnIncrementArrow->VSetSize(cVector2(height, height));
		backGroundWidth -= height;
	}
	if (m_pBGSprite)
	{
		m_pBGSprite->VSetSize(cVector2(backGroundWidth, VGetSize().y));
	}

	if (m_pBtnThumb)
	{
		VAutoSizeThumb();
	}
	VSetAbsolutePosition();
}

//  *******************************************************************************************************************
void cHScrollBar::VSetThumbPosition(const int iNewPosition)
{
	cScrollBarControl::VSetThumbPosition(iNewPosition);

	if (m_pBtnThumb)
	{
		cVector2 pos = m_vControlAbsolutePosition;
		pos.x += m_pBtnDecrementArrow->VGetSize().x + (m_pBtnThumb->VGetSize().x * m_ThumbPos);
		m_pBtnThumb->VSetPosition(pos);
	}
	SP_LOG(3, cStringUtilities::MakeFormatted("ThumbPos % d" , (m_ThumbPos + m_MinPos)));
}

//  *******************************************************************************************************************
void cHScrollBar::VAutoSizeThumb()
{
	float thumbRange = VGetSize().x - m_pBtnIncrementArrow->VGetSize().x - m_pBtnDecrementArrow->VGetSize().x;
	float newThumbWidth = thumbRange / (m_NoOfIncrements + 1);
	m_pBtnThumb->VSetSize(cVector2(newThumbWidth, VGetSize().y));
}
