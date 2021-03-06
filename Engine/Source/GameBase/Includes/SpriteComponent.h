//  **********************************************************************************************************************************************************
//  SpriteComponent   version:  1.0   Ankur Sheel  date: 2013/04/08
//  **********************************************************************************************************************************************************
//  purpose:
//  **********************************************************************************************************************************************************
#ifndef SpriteComponent_h__
#define SpriteComponent_h__

#include "BaseComponent.h"
#include "GameBaseDefines.h"
#include "RenderableComponent.hxx"

namespace Utilities
{
  class IXMLNode;
}

namespace Graphics
{
  class ISprite;
  class ICamera;
}

namespace Base
{
  class cVector3;

  template <class BaseType, class SubType>
  shared_ptr<BaseType> GenericObjectCreationFunction();

  template <class BaseType, class SubType>
  shared_ptr<BaseType> GenericObjectDuplicationFunction(shared_ptr<BaseType> const pObject);
}

namespace GameBase
{
  class cSpriteComponent : public cBaseComponent, public IRenderableComponent
  {
  public:
    cSpriteComponent(const cSpriteComponent& other);
    ~cSpriteComponent();
    GAMEBASE_API void VInitialize(const Utilities::IXMLNode* const pXMLNode) OVERRIDE;
    GAMEBASE_API void VCleanup() OVERRIDE;
    void VSetPosition(const Base::cVector3& position) OVERRIDE;
    void VSetRotation(const Base::cVector3& rotation) OVERRIDE;
    void VSetSize(const Base::cVector3& size) OVERRIDE;
    void VRender(const Graphics::ICamera* const pCamera) OVERRIDE;
    void VGetBounds(Base::cVector3& minBound, Base::cVector3& maxBounds) OVERRIDE;
    shared_ptr<Utilities::IXMLNode> VGenerateXml() const OVERRIDE;

    GAMEBASE_API static Base::cHashedString GetName() { return m_Name; }
  private:
    cSpriteComponent();
    cSpriteComponent& operator=(const cSpriteComponent& other);
    UINT64 VGetHashedID() const { return m_Name.GetHash(); }
  private:
    GAMEBASE_API static Base::cHashedString m_Name;  ///< The component name
    Graphics::ISprite* m_pSprite;                    ///< The 2d Sprite
    Base::cString m_SpriteName;

  private:
    template <class BaseType, class SubType>
    friend shared_ptr<BaseType> Base::GenericObjectCreationFunction();

    template <class BaseType, class SubType>
    friend shared_ptr<BaseType> Base::GenericObjectDuplicationFunction(shared_ptr<BaseType> const pObject);
  };
}  // namespace GameBase
#endif  // SpriteComponent_h__
