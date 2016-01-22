//  *******************************************************************************************************************
//  RenderSystem version:  1.0   Ankur Sheel  date: 2013/04/10
//  *******************************************************************************************************************
//  purpose:
//  *******************************************************************************************************************
#ifndef RenderSystem_h__
#define RenderSystem_h__

#include "GameBaseDefines.h"
#include "Process.h"

namespace Graphics
{
	class ICamera;
}

namespace GameBase
{
	class IEventData;
	typedef shared_ptr<IEventData> IEventDataPtr;
	class IEntityManager;
	class IRenderableComponent;
	class IBaseEntity;
}

namespace GameBase
{
	class cRenderSystem
		: public Utilities::cProcess
	{
	public:
		GAMEBASE_API cRenderSystem();
		GAMEBASE_API ~cRenderSystem();
		void VInitialize();
		const Base::cHashedString & VGetType() const { return m_Type; }
		GAMEBASE_API void Render(const Graphics::ICamera * const pCamera);
		void ActorMovedListener(IEventDataPtr pEventData);
		void ActorRotatedListener(IEventDataPtr pEventData);
		void ActorScaledListener(IEventDataPtr pEventData);

	public:
		GAMEBASE_API static const Base::cHashedString	m_Type;

	private:
		shared_ptr<IRenderableComponent> const CastToRenderableComponent(shared_ptr<IBaseEntity> const pEntity);

	private:
		weak_ptr<IEntityManager> m_pEntityManager;
	};
}  // namespace GameBase
#endif  // RenderSystem_h__
