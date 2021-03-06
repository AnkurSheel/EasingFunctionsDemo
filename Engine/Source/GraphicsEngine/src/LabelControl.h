//  *******************************************************************************************************************
//  LabelControl   version:  1.0   Ankur Sheel  date: 2011/11/22
//  *******************************************************************************************************************
//  purpose:
//  *******************************************************************************************************************
#ifndef LabelControl_h__
#define LabelControl_h__

#include "BaseControl.h"

namespace Base
{
  template <class BaseType, class SubType>
  shared_ptr<BaseType> GenericObjectCreationFunction();

  template <class BaseType, class SubType>
  shared_ptr<BaseType> GenericObjectDuplicationFunction(shared_ptr<BaseType> const pObject);
}

namespace Graphics
{
  class IMyFont;
  class ISentence;
}

namespace Graphics
{
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  /// @brief Class Declaration for a Label UI Control
  ///
  ///
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  class cLabelControl : public cBaseControl
  {
  public:
    static Base::cHashedString GetName() { return m_Name; }
    cLabelControl(const cLabelControl& other);
    ~cLabelControl();

  private:
    cLabelControl();
    void VInitialize(const shared_ptr<Utilities::IXMLNode const> pXMLNode);
    shared_ptr<IBaseControl> VDuplicate();
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Initializes the label control as per the parameters
    ///
    /// @param[in] def The parameter definition to create a label control
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void Initialize(const cLabelControlDef& def);
    void VRender(const ICamera* const pCamera);
    void VSetAbsolutePosition();
    void VSetText(const Base::cString& strText);
    UINT64 VGetHashedID() const { return m_Name.GetHash(); }
    Base::cString VToXml() const OVERRIDE;

    void VCleanup();

  private:
    ISentence* m_pSentence;  /// The text/sentence of the label.
    Base::cColor m_TextColor;
    Base::cString m_FontName;
    static Base::cHashedString m_Name;  /// The control name

  private:
    template <class BaseType, class SubType>
    friend shared_ptr<BaseType> Base::GenericObjectCreationFunction();

    template <class BaseType, class SubType>
    friend shared_ptr<BaseType> Base::GenericObjectDuplicationFunction(shared_ptr<BaseType> const pObject);

    friend class IBaseControl;
  };
}  // namespace Graphics
#endif  // LabelControl_h__
