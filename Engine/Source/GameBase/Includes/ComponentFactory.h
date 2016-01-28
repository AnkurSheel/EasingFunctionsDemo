//  *******************************************************************************************************************
//  ComponentFactory   version:  1.0   Ankur Sheel  date: 2013/05/15
//  *******************************************************************************************************************
//  purpose:
//  *******************************************************************************************************************

#ifndef ComponentFactory_h__
#define ComponentFactory_h__

#include "GameBaseDefines.h"
#include "GenericObjectCreation.h"

namespace GameBase
{
  class IBaseComponent;
}

namespace GameBase
{
  class cComponentFactory
  {
  public:
    GAMEBASE_API cComponentFactory();
    GAMEBASE_API ~cComponentFactory();
    GAMEBASE_API void VRegisterComponents();
    shared_ptr<IBaseComponent> CreateComponent(const Base::cHashedString& Type);
    GAMEBASE_API shared_ptr<IBaseComponent> GetDuplicateComponent(shared_ptr<IBaseComponent> const pComponent);
    static cComponentFactory* Instance() { return m_pInstance; }
  private:
    Base::GenericObjectFactory<IBaseComponent, UINT64> m_RegisteredComponents;
    GAMEBASE_API static cComponentFactory* m_pInstance;
  };
}  // namespace GameBase
#endif  // ComponentFactory_h__
