//  *******************************************************************************************************************
//  EntityScaledEventData version:  1.0   Ankur Sheel  date: 2013/07/25
//  *******************************************************************************************************************
//  purpose:
//  *******************************************************************************************************************
#ifndef EntityScaledEventData_h__
#define EntityScaledEventData_h__

#include "BaseEventData.h"
#include "vector3.h"

namespace GameBase
{
	class cEntityScaledEventData
		: public cBaseEventData
	{
	public:
		cEntityScaledEventData();
		cEntityScaledEventData(const Base::cVector3 & size, const int actorID);
		~cEntityScaledEventData();
		UINT64 VGetHashedID() const { return m_Name.GetHash(); }
		virtual Base::cString VGetName() const { return m_Name.GetString(); }
		int GetActorID() const { return m_ActorID; }
		Base::cVector3 GetSize() const { return m_Size; }

	public:
		static Base::cHashedString m_Name;

	private:
		Base::cVector3	m_Size;
		int				m_ActorID;
	};
}  // namespace GameBase
#endif  // EntityScaledEventData_h__