//  *******************************************************************************************************************
//  EntityFactory   version:  1.0   Ankur Sheel  date: 2013/05/06
//  *******************************************************************************************************************
//  purpose:
//  *******************************************************************************************************************

#ifndef EntityFactory_h__
#define EntityFactory_h__

#include "GameBaseDefines.h"
#include "GenericObjectCreation.h"

namespace GameBase
{
  class IBaseEntity;
  class IBaseComponent;
}

namespace GameBase
{
  class cEntityFactory : public Base::cNonCopyable
  {
  public:
    GAMEBASE_API cEntityFactory();
    GAMEBASE_API virtual ~cEntityFactory();
    GAMEBASE_API virtual void VRegisterEntities() = 0;
    GAMEBASE_API virtual shared_ptr<IBaseEntity> VCreateEntity(const Base::cHashedString& Type);
    GAMEBASE_API static cEntityFactory* Instance() { return m_pInstance; }
  protected:
    Base::GenericObjectFactory<IBaseEntity, UINT64> m_RegisteredEntities;
    GAMEBASE_API static cEntityFactory* m_pInstance;
  };
}  // namespace GameBase
#endif  // EntityFactory_h__
