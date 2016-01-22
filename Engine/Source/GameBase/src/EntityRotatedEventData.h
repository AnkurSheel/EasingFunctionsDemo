//  *******************************************************************************************************************
//  EntityRotatedEventData   version:  1.0   Ankur Sheel  date: 2015/04/20
//  *******************************************************************************************************************
//
//  *******************************************************************************************************************
#ifndef ENTITYROTATEDEVENTDATA_H__
#define ENTITYROTATEDEVENTDATA_H__

#include "BaseEventData.h"
#include "vector3.h"

namespace GameBase
{
	class cEntityRotatedEventData
		: public cBaseEventData
	{
	public:
		cEntityRotatedEventData();
		cEntityRotatedEventData(const Base::cVector3 & eulerAngles, const int actorID);
		~cEntityRotatedEventData();
		UINT64 VGetHashedID() const OVERRIDE { return m_Name.GetHash(); }
		virtual Base::cString VGetName() const OVERRIDE { return m_Name.GetString(); }
		int GetActorID() const { return m_ActorID; }
		Base::cVector3 GetRotation() const { return m_Rotation; }

	public:
		static Base::cHashedString m_Name;

	private:
		Base::cVector3 m_Rotation;
		int m_ActorID;
	};
}  // namespace GameBase
#endif  // ENTITYROTATEDEVENTDATA_H__





