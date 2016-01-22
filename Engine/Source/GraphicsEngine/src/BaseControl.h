//  *******************************************************************************************************************
//  BaseControl   version:  1.0   Ankur Sheel  date: 2011/07/21
//  *******************************************************************************************************************
//  purpose:
//  *******************************************************************************************************************
#ifndef BaseControl_h__
#define BaseControl_h__

#include "BaseControl.hxx"

namespace Base
{
	struct AppMsg;
	class cHashedString;
}

namespace Graphics
{
	class ISprite;
}

namespace Utilities
{
	class IXMLNode;
}

namespace Graphics
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief Encapsulates all the base functionality required by all the UI controls
	///
	///
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class cBaseControl
		: public IBaseControl
		, public Base::cNonCopyable
	{
	public:
		virtual ~cBaseControl();
		void VSetVisible(bool visible) OVERRIDE;
		const Base::cVector2& VGetSize() const OVERRIDE { return m_Size; }
		const Base::cString& VGetControlName() const OVERRIDE;
		void VDeRegisterAllCallBacks() OVERRIDE;
		void VRender(const ICamera* const pCamera) OVERRIDE;
		bool VPostMsg(const Base::AppMsg& msg) OVERRIDE;
		int VGetNumberOfChildControls(bool recursive) const OVERRIDE { return 0; }
		UINT64 VGetControlNameHash() const OVERRIDE { return m_strControlName.GetHash(); }
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// Sets the absolute position of the control relative to the screen.
		///
		///
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual void VSetAbsolutePosition();
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// Sets the parent control of the current control
		///
		/// @param[in] pParentControl Pointer to the parent control
		///
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void SetParentControl(cBaseControl * pParentControl) { m_pParentControl = pParentControl; }
		bool GetFocus() const { return m_bFocus; }
		void SetUIRootControl(weak_ptr<IBaseControl> pUiRootControl) { m_pUIRootControl = pUiRootControl; }

	protected:
		///< List of controls.
		typedef std::list<shared_ptr<cBaseControl> >  ControlList;

		cBaseControl();
		cBaseControl(const cBaseControl & other);

		virtual UINT64 VGetHashedID() const {return 0;}  // remove later
		void VInitialize(const shared_ptr<Utilities::IXMLNode const> pXMLNode) OVERRIDE;
		void Initialize(const cBaseControlDef& def);  // remove later
		bool VOnLeftMouseButtonUp(const int X, const int Y) OVERRIDE;
		bool VOnLeftMouseButtonDown(const int X, const int Y) OVERRIDE;
		bool VOnMouseMove(const int X, const int Y) OVERRIDE;
		bool VOnKeyDown(const unsigned int iCharID) OVERRIDE;
		bool VOnCharPress(const unsigned int iCharID) OVERRIDE;
		void VAddChildControl(shared_ptr<IBaseControl> pChildControl, weak_ptr<IBaseControl> pUiRootWindowControl) OVERRIDE;
		void VSetSize(const Base::cVector2& size) OVERRIDE;
		void VDeregisterCallBack(const UIEVENTTYPE::ENUM eventType) OVERRIDE;
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// Event Handler for when the control gains/loses focus.
		///
		///
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual void VOnFocusChanged();
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// Releases and destroys all the resources/pointers
		///
		///
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual void VCleanup();
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// Sets the focus control to pControl
		///
		/// @param[in] pControl The control which has the focus
		///
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void SetFocusControl(const cBaseControl * const pControl);
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// Sets the focus.
		///
		/// @param[in] bFocus True if the controls gains focus. False if the control loses focus
		///
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void SetFocus(const bool bFocus);
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// Returns The callback function associated with this event.  If the function is not found, it will return null
		///
		/// @param[in] eventType The type of the event for the call back.
		/// @return The callback function associated with this event
		///
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		UIEventCallBackFn * GetCallbackFromMap(const UIEVENTTYPE::ENUM eventType);
		Base::cVector2 GetUIRatio() const;
		void InitializePosition(const shared_ptr<Utilities::IXMLNode const> pXMLNode);
		void GenerateXml(shared_ptr<Utilities::IXMLNode> pXmlNode) const;

	private:
		///< Map of callback functions with the event ype as the key.
		typedef std::map<UIEVENTTYPE::ENUM, UIEventCallBackFn >  UIEventCallBackMap;

		bool VOnKeyUp(const unsigned int iCharID) OVERRIDE;
		void VRemoveAllChildren() OVERRIDE;
		void VRemoveChildControl(const Base::cString& controlName) OVERRIDE;
		const shared_ptr<IBaseControl> VFindChildControl(const Base::cString& controlName) OVERRIDE;
		const shared_ptr<IBaseControl> VFindChildControl(const UINT64 hashedID) OVERRIDE;
		const shared_ptr<IBaseControl> VGetChildControlAtIndex(const int index) OVERRIDE;
		Base::cString VToXml() const OVERRIDE;
		void VSetText(const Base::cString& text) OVERRIDE;
		void VSetPosition(const Base::cVector2& position) OVERRIDE;
		void VRegisterCallBack(const UIEVENTTYPE::ENUM eventType, UIEventCallBackFn fnCallback) OVERRIDE;
		void VMoveToFront(const IBaseControl* const pControl) OVERRIDE;
		void VSetControlName(const Base::cString& inControlName) OVERRIDE;
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// Returns True if the user can move the controls using the mouse. False otherwise
		///
		/// @return True if the user can move the controls using the mouse. False otherwise
		///
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool AllowMovingControl();
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// Constrain child control in parent control
		///
		/// @param[in] fX The X position of the control
		/// @param[in] fY The Y position of the control
		///
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void ConstrainChildControl(float & fX, float & fY);
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// Posts the msg to all the child controls
		///
		/// @param[in] msg The windows message
		/// @return True if the child control handles the message. False otherwise
		///
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual bool VPostToAll(const Base::AppMsg & msg);
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// Returns True if the mouse intersects this control. False otherwise
		///
		/// @param[in] fX The X position of the mouse
		/// @param[in] fY The Y position of the mouse
		/// @return True if the mouse intersects this control. False otherwise
		///
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool IsCursorIntersect(const float fX, const float fY);

	protected:
		ISprite * m_pBGSprite;  ///< The background sprite associated with this control.
		Base::cVector2 m_vControlAbsolutePosition;  ///< The absolute position of the control on the screen.
		Base::cVector2 m_Size;  ///< The size of the control.
		Base::cVector2 m_vPosition;  ///< The position of the control w.r.t to its parent control.
		UIEventCallBackMap m_CallbackMap;  ///< The map containing all the callback functions with the event type as the key
		bool m_bVisible;  ///< True if the control is visible. False otherwise.
		bool m_bFocus;  ///< True if the control or any of its child controls has focus. False otherwise
		bool m_bAllowMovingControls;  ///< True if the user can move the controls using the mouse. False otherwise.
		bool m_bIsLeftMouseDown;  ///< True if the left mouse button is pressed.
		cBaseControl* m_pParentControl;  ///< The parent control.
		cBaseControl* m_pFocusControl;  ///< The control which has the focus.
		Base::cHashedString m_strControlName;  ///< The name of the control.
		weak_ptr<IBaseControl> m_pUIRootControl;

	private:
		int m_iMouseDownXPos;  ///< The X position of the mouse relative to the absolute control position.
		int m_iMouseDownYPos;  ///< The Y position of the mouse relative to the absolute control position.
		bool m_ConstrainInParent;
	};
}  // namespace Graphics
#endif  // BaseControl_h__
