//  *******************************************************************************************************************
//  WindowControl   version:  1.0   Ankur Sheel  date: 2011/11/21
//  *******************************************************************************************************************
//  purpose:
//  *******************************************************************************************************************
#ifndef WindowControl_h__
#define WindowControl_h__

#include "BaseControl.h"

namespace Base
{
	template<class BaseType, class SubType>
	shared_ptr<BaseType> GenericObjectCreationFunction();

	template <class BaseType, class SubType>
	shared_ptr<BaseType> GenericObjectDuplicationFunction(shared_ptr<BaseType> const pObject);
}

namespace Graphics
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// Class Declaration for a Window UI Control
	///
	///
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class cWindowControl
		: public cBaseControl
	{
	public:
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// Initializes the window control as per the parameters
		///
		/// @param[in] def The parameter definition to create a window control
		///
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void Initialize(const cWindowControlDef & def);
		static Base::cHashedString	GetName()  {return m_Name; }
		~cWindowControl();
		cWindowControl(const cWindowControl & other);

	private:
		cWindowControl();
		void VInitialize(const shared_ptr<Utilities::IXMLNode const> pXMLNode) OVERRIDE;
		shared_ptr<IBaseControl> VDuplicate() OVERRIDE;
		bool VOnLeftMouseButtonUp(const int X, const int Y) OVERRIDE;
		bool VOnLeftMouseButtonDown(const int X, const int Y) OVERRIDE;
		bool VOnMouseMove(const int X, const int Y) OVERRIDE;
		void VSetAbsolutePosition() OVERRIDE;
		UINT64 VGetHashedID() const  OVERRIDE { return m_Name.GetHash(); }
		void VAddChildControl(shared_ptr<IBaseControl> pChildControl, weak_ptr<IBaseControl> pUiRootWindowControl) OVERRIDE;
		void VRemoveAllChildren() OVERRIDE;
		void VMoveToFront(const IBaseControl * const pControl) OVERRIDE;
		void VRender(const ICamera * const pCamera) OVERRIDE;
		const shared_ptr<Graphics::IBaseControl> VFindChildControl(const Base::cString & strControlName) OVERRIDE;
		const shared_ptr<IBaseControl> VFindChildControl(const UINT64 hashedID) OVERRIDE;
		const shared_ptr<IBaseControl> VGetChildControlAtIndex(const int index) OVERRIDE;
		void VRemoveChildControl(const Base::cString & strControlName) OVERRIDE;
		bool VPostToAll(const Base::AppMsg & msg) OVERRIDE;
		void VCleanup() OVERRIDE;
		int VGetNumberOfChildControls(bool recursive) const OVERRIDE;
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// Returns a const_iterator if the child control is found in the list. If the child control is not found,
		//  the const_iterator will be equal list.end();
		///
		/// @param[in] pChildControl The child control for which we want the iterator
		/// @return a const_iterator if the child control is found in the list.
		///
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		ControlList::const_iterator GetChildControlIterator(const IBaseControl * const pChildControl);

	private:
		GRAPHIC_API static Base::cHashedString m_Name;  ///< The control name
		cWindowControlDef::WINDOWTYPE m_eWindowType;  ///< The window type.
		ControlList m_pChildControls;  ///< List of child controls.

	private:
		template<class BaseType, class SubType>
		friend shared_ptr<BaseType> Base::GenericObjectCreationFunction();

		template <class BaseType, class SubType>
		friend shared_ptr<BaseType> Base::GenericObjectDuplicationFunction(shared_ptr<BaseType> const pObject);

		friend class IBaseControl;
	};
}  // namespace Graphics
#endif  // WindowControl_h__
