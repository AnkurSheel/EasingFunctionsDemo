#include "stdafx.h"
#include "VScrollBar.h"
#include "Sprite.hxx"

using namespace Graphics;
using namespace Utilities;
using namespace Base;

cHashedString cVScrollBar::m_Name = cHashedString("verticalscrollbar");

//  *******************************************************************************************************************
cVScrollBar::cVScrollBar()
{
}

//  *******************************************************************************************************************
cVScrollBar::~cVScrollBar()
{
}

//  *******************************************************************************************************************
shared_ptr<IBaseControl> cVScrollBar::VDuplicate()
{
  return std::shared_ptr<IBaseControl>(DEBUG_NEW cVScrollBar(*this));
}

//  *******************************************************************************************************************
bool cVScrollBar::VOnMouseMove(const int X, const int Y)
{
  if (m_DragMode)
  {
    int thumbPos = m_ThumbPos;

    // get the position of the mouse in the scrollbar
    float posRelativeToScrollbar =
        m_pBtnDecrementArrow->VGetSize().y + Y - (m_vControlAbsolutePosition.y + m_pBtnIncrementArrow->VGetSize().y);

    // check if the mouse position is still within the scrollbar
    if (posRelativeToScrollbar >= m_pBtnIncrementArrow->VGetSize().y &&
        posRelativeToScrollbar < (VGetSize().y - m_pBtnDecrementArrow->VGetSize().y))
    {
      // get the area in which the thumb can move
      float thumbRange = (VGetSize().y - m_pBtnIncrementArrow->VGetSize().y - m_pBtnDecrementArrow->VGetSize().y);

      float pixelsMovedPerIncrement = thumbRange / m_NoOfIncrements;

      // get the cuurent pos of the thum
      for (int counter = 0; counter < m_NoOfIncrements; counter++)
      {
        // get the current pos for this counter
        float posForCounter = m_pBtnDecrementArrow->VGetSize().y + (pixelsMovedPerIncrement * counter);

        if (((posRelativeToScrollbar >= posForCounter)) &&
            (posRelativeToScrollbar < (posForCounter + pixelsMovedPerIncrement)))
        {
          thumbPos = counter;
          break;
        }
      }
    }
    VSetThumbPosition(thumbPos);
    return true;
  }
  return cBaseControl::VOnMouseMove(X, Y);
}

//  *******************************************************************************************************************
void cVScrollBar::VSetAbsolutePosition()
{
  cScrollBarControl::VSetAbsolutePosition();
  cVector2 pos = m_vControlAbsolutePosition;
  if (m_pBtnDecrementArrow)
  {
    m_pBtnDecrementArrow->VSetPosition(pos);
    if (m_pBGSprite)
    {
      pos.y += m_pBtnDecrementArrow->VGetSize().y;
      m_pBGSprite->VSetPosition(pos);
    }
  }
  if (m_pBtnIncrementArrow)
  {
    pos.y = m_vControlAbsolutePosition.y + VGetSize().y - m_pBtnIncrementArrow->VGetSize().y;
    m_pBtnIncrementArrow->VSetPosition(pos);
  }
  VSetThumbPosition(m_ThumbPos);
}

//  *******************************************************************************************************************
void cVScrollBar::VSetSize(const cVector2& vSize)
{
  cBaseControl::VSetSize(vSize);
  float fBackGroundHeight = VGetSize().y;
  float fWidth = VGetSize().x;
  if (m_pBtnDecrementArrow)
  {
    m_pBtnDecrementArrow->VSetSize(cVector2(fWidth, fWidth));
    fBackGroundHeight -= fWidth;
  }
  if (m_pBtnIncrementArrow)
  {
    m_pBtnIncrementArrow->VSetSize(cVector2(fWidth, fWidth));
    fBackGroundHeight -= fWidth;
  }
  if (m_pBGSprite)
  {
    m_pBGSprite->VSetSize(cVector2(VGetSize().x, fBackGroundHeight));
  }
  if (m_pBtnThumb)
  {
    VAutoSizeThumb();
  }
  VSetAbsolutePosition();
}

//  *******************************************************************************************************************
void cVScrollBar::VSetThumbPosition(const int iNewPosition)
{
  cScrollBarControl::VSetThumbPosition(iNewPosition);

  if (m_pBtnThumb)
  {
    cVector2 pos = m_vControlAbsolutePosition;
    pos.y += m_pBtnDecrementArrow->VGetSize().y + (m_pBtnThumb->VGetSize().y * m_ThumbPos);
    m_pBtnThumb->VSetPosition(pos);
  }
  SP_LOG(3, cStringUtilities::MakeFormatted("ThumbPos % d", (m_ThumbPos + m_MinPos)));
}

//  *******************************************************************************************************************
void cVScrollBar::VAutoSizeThumb()
{
  float thumbRange = VGetSize().y - m_pBtnIncrementArrow->VGetSize().y - m_pBtnDecrementArrow->VGetSize().y;
  float newThumbHeight = thumbRange / m_NoOfIncrements;
  m_pBtnThumb->VSetSize(cVector2(VGetSize().x, newThumbHeight));
}
