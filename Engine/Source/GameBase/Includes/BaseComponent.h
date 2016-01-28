//  *******************************************************************************************************************
//  BaseComponent version:  1.0   Ankur Sheel  date: 2013/04/02
//  *******************************************************************************************************************
//  purpose:
//  *******************************************************************************************************************
#ifndef BaseComponent_h__
#define BaseComponent_h__

#include "BaseComponent.hxx"

namespace GameBase
{
  class IBaseEntity;
}

namespace GameBase
{
  class cBaseComponent : public IBaseComponent
  {
  public:
    cBaseComponent();
    cBaseComponent(const cBaseComponent& other);
    cBaseComponent& operator=(const cBaseComponent& other);
    GAMEBASE_API virtual ~cBaseComponent();

    UINT64 VGetID() const OVERRIDE { return VGetHashedID(); }
    void VOnAttached(shared_ptr<IBaseEntity> const pOwner) OVERRIDE;

  protected:
    virtual UINT64 VGetHashedID() const = 0;
    virtual void VSetActive(const bool active) OVERRIDE { m_Active = active; }
    virtual bool VIsActive() const OVERRIDE { return m_Active; }
  protected:
    weak_ptr<IBaseEntity> m_pOwner;

  private:
    bool m_Active;
  };
}  // namespace GameBase
#endif  // BaseComponent_h__
