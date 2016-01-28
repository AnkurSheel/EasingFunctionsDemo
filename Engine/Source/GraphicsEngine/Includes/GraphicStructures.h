//  *******************************************************************************************************************
//  GraphicStructures   version:  1.0   Ankur Sheel  date: 2012/10/17
//  *******************************************************************************************************************
//
//  *******************************************************************************************************************
#ifndef GraphicStructures_h__
#define GraphicStructures_h__

#include "Color.h"
#include "vector2.h"

namespace Graphics
{
  class IBaseControl;
}

namespace Graphics
{
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /// Callback Event Types for UI controls
  ///
  ///
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  namespace UIEVENTTYPE
  {
    enum ENUM
    {
      BUTTONPRESSED,     /// Button was pressed
      BUTTONRELEASED,    /// Button was released
      SCROLLBARCHANGED,  /// The value of the Scrollbar was changed
      FOCUSLOST,         /// The UI control lost focus
      KEYDOWN,           /// A key was pressed
    };
  }  // namespace UIEVENTTYPE

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /// Return structure for UI callback functions
  ///
  ///
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  struct stUIEventCallbackParam
  {
    bool bChecked;          /// Will be set/unset for checkboxes
    int iThumbPos;          /// The position of the thumb for scrollbars
    unsigned int uiCharId;  /// The ascii code of the key pressed/released
    Base::cString strText;  /// The text for textboxes

    stUIEventCallbackParam()
      : bChecked(false)
      , iThumbPos(0)
      , uiCharId(0)
    {
    }
  };

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /// Common parameters for creating a UI control
  ///
  ///
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  class cBaseControlDef
  {
  public:
    Base::cVector2 vPosition;      /// The position of the control
    Base::cVector2 vSize;          /// The scale in pixels of the control
    Base::cString strControlName;  /// The name of the control
  };

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /// Params for creating a window control
  ///
  ///
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  class cWindowControlDef : public cBaseControlDef
  {
  public:
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Enum for setting the window type for window controls
    ///
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    enum WINDOWTYPE
    {
      WT_DESKTOP,   /// This type contains the whole app screen. All other UI items will be a child of this
      WT_STANDARD,  /// This type is used to set a child window control. Can be used for grouping controls
    };

  public:
    WINDOWTYPE wType;              /// The window type. Can be DESKTOP or STANDARD
    Base::cString strBGImageFile;  /// The path for the background image. Can be Empty
    bool bAllowMovingControls;     /// True if we want to allow the users to change the position of the control

    cWindowControlDef()
      : wType(WT_DESKTOP)
      , bAllowMovingControls(false)
    {
    }
  };

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /// Definition for creating a label control
  ///
  ///
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  class cLabelControlDef : public cBaseControlDef
  {
  public:
    Base::cString strFont;         /// The font that will be used to display the text
    Base::cColor textColor;        /// The text color
    Base::cString strText;         /// The text that should be displayed initially
    Base::cString strBGImageFile;  /// The path for the background image. Can be Empty */
    float fTextHeight;             /// The text height
    bool bAutoSize;                /// If true, autosizes the label based on the text width and height

    cLabelControlDef()
      : fTextHeight(0.0f)
      , bAutoSize(true)
    {
    }
  };

  struct stInitializationParameters
  {
    HWND windowHandle;     /// A handle to the current window
    float screenDepth;     /// Far Depth setting for the 3D environment
    float screenNear;      /// Near Depth setting for the 3D environment
    Base::cColor bkColor;  /// The default background color of the window
    int width;             /// Width of the window
    int height;            /// Height of the window
    bool fullScreen;       /// True if the application is full screen. False if Windowed.
    bool vsyncEnabled;  /// True if we want Direct3D to render according to the users monitor refresh rate. False if we
                        /// want to  render as fast as possible.
  };

}  // namespace Graphics
#endif  // GraphicStructures_h__
