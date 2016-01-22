#include "stdafx.h"
#include "EntityRotatedEventData.h"

using namespace GameBase;
using namespace Base;

cHashedString	cEntityRotatedEventData::m_Name = cHashedString("evententityrotated");

//  *******************************************************************************************************************
cEntityRotatedEventData::cEntityRotatedEventData()
	: m_ActorID(0)
{
}

//  *******************************************************************************************************************
cEntityRotatedEventData::cEntityRotatedEventData(const cVector3 & eulerAngles, const int actorID)
	: m_ActorID(actorID)
	, m_Rotation(eulerAngles)
{
}

//  *******************************************************************************************************************
cEntityRotatedEventData::~cEntityRotatedEventData()
{
}



