//  *******************************************************************************************************************
//  EventFactory   version:  1.0   Ankur Sheel  date: 2013/07/25
//  *******************************************************************************************************************
//  purpose:
//  *******************************************************************************************************************

#ifndef EventFactory_h__
#define EventFactory_h__

#include "GameBaseDefines.h"
#include "GenericObjectCreation.h"
#include "EventData.hxx"

namespace GameBase
{
	class cEventFactory
	{
	public:
		GAMEBASE_API cEventFactory();
		GAMEBASE_API ~cEventFactory();
		GAMEBASE_API void VRegisterEvents();
		IEventData * CreateEvent(const EventType & Type);
		static cEventFactory * Instance() { return m_pInstance; }

	private:
		Base::GenericObjectFactory<IEventData, UINT64>	m_RegisteredEvents;
		GAMEBASE_API static cEventFactory *				m_pInstance;
	};
}  // namespace GameBase
#endif  // EventFactory_h__
