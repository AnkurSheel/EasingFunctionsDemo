//  *******************************************************************************************************************
//  UiControlFactory   version:  1.0   Ankur Sheel  date: 2013/05/15
//  *******************************************************************************************************************
//  purpose:
//  *******************************************************************************************************************

#ifndef UiControlFactory_h__
#define UiControlFactory_h__

#include "GenericObjectCreation.h"

namespace Graphics
{
  class IBaseControl;
}

namespace Graphics
{
  class cUiControlFactory
  {
  public:
    ~cUiControlFactory();
    void VRegisterUiControls();
    shared_ptr<IBaseControl> CreateUiControl(const Base::cHashedString& Type);
    static cUiControlFactory* Instance();
    static void Destroy();

  private:
    cUiControlFactory();

  private:
    Base::GenericObjectFactory<IBaseControl, UINT64> m_RegisteredControls;
    static cUiControlFactory* m_pInstance;
  };
}  // namespace Graphics
#endif  // UiControlFactory_h__
