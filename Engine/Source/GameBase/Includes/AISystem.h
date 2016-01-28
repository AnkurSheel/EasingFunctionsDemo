//  *******************************************************************************************************************
//  AISystem   version:  1.0   Ankur Sheel  date: 2015/05/07
//  *******************************************************************************************************************
//
//  *******************************************************************************************************************
#ifndef AISYSTEM_H__
#define AISYSTEM_H__

#include "GameBaseDefines.h"
#include "Process.h"

namespace GameBase
{
  class IEntityManager;
  class IBaseEntity;
  class cAIComponent;
}

namespace GameBase
{
  class cAISystem : public Utilities::cProcess
  {
  public:
    GAMEBASE_API cAISystem();
    GAMEBASE_API ~cAISystem();
    void VInitialize() OVERRIDE;
    void VUpdate(const float deltaTime) OVERRIDE;
    const Base::cHashedString& VGetType() const OVERRIDE { return m_Type; }
  public:
    GAMEBASE_API static const Base::cHashedString m_Type;

  private:
    shared_ptr<cAIComponent> const CastToAIComponent(shared_ptr<IBaseEntity> const pEntity);

  private:
    weak_ptr<IEntityManager> m_pEntityManager;
  };
}  // namespace GameBase
#endif  // AISYSTEM_H__
