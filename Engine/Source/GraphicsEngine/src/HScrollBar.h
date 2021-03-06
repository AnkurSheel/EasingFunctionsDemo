//  *******************************************************************************************************************
//  HScrollBar   version:  1.0   Ankur Sheel  date: 2012/04/29
//  *******************************************************************************************************************
//
//  *******************************************************************************************************************
#ifndef HScrollBar_h__
#define HScrollBar_h__

#include "ScrollBarControl.h"

namespace Base
{
  template <class BaseType, class SubType>
  shared_ptr<BaseType> GenericObjectCreationFunction();
}

namespace Graphics
{
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /// @brief Class Declaration for a Horizontal Scrollbar UI Control
  ///
  ///
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  class cHScrollBar : public cScrollBarControl
  {
  public:
    static Base::cHashedString GetName() { return m_Name; }
    ~cHScrollBar();

  private:
    cHScrollBar();
    shared_ptr<IBaseControl> VDuplicate();
    bool VOnMouseMove(const int X, const int Y);
    void VSetAbsolutePosition();
    void VSetSize(const Base::cVector2& vSize);
    void VSetThumbPosition(const int iNewPosition);
    void VAutoSizeThumb();

  private:
    static Base::cHashedString m_Name;  ///< The control name

  private:
    template <class BaseType, class SubType>
    friend shared_ptr<BaseType> Base::GenericObjectCreationFunction();
  };
}  // namespace Graphics
#endif  // HScrollBar_h__
