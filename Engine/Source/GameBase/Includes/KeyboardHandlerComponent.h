//  *******************************************************************************************************************
//  KeyboardHandlerComponent version:  1.0   Ankur Sheel  date: 2013/05/16
//  *******************************************************************************************************************
//  purpose:
//  *******************************************************************************************************************
#ifndef KeyboardHandlerComponent_h__
#define KeyboardHandlerComponent_h__

#include "BaseComponent.h"
#include "GameBaseDefines.h"

namespace Base
{
  template <class BaseType, class SubType>
  shared_ptr<BaseType> GenericObjectCreationFunction();
}

namespace GameBase
{
  class cKeyboardHandlerComponent : public cBaseComponent
  {
  public:
    ~cKeyboardHandlerComponent();
    GAMEBASE_API void VInitialize(const Utilities::IXMLNode* const pXMLNode) OVERRIDE;
    GAMEBASE_API void VCleanup() OVERRIDE;
    shared_ptr<Utilities::IXMLNode> VGenerateXml() const OVERRIDE;
    GAMEBASE_API static Base::cHashedString GetName() { return m_Name; }
    void Update(const float deltaTime);

  private:
    UINT64 VGetHashedID() const { return m_Name.GetHash(); }
    cKeyboardHandlerComponent();

  private:
    static Base::cHashedString m_Name;  ///< The component name

  private:
    template <class BaseType, class SubType>
    friend shared_ptr<BaseType> Base::GenericObjectCreationFunction();
  };
}  // namespace GameBase
#endif  // KeyboardHandlerComponent_h__
