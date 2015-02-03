//  *******************************************************************************************************************
//  EasingDemoEntityFactory version: 1.0 Ankur Sheel  date: 2015/02/03
//  *******************************************************************************************************************
//  purpose:	
//  *******************************************************************************************************************
#ifndef EasingDemoEntityFactory_H__
#define EasingDemoEntityFactory_H__

#include "EntityFactory.h"
#include "GameDefines.h"

namespace GameBase
{
	class IBaseEntity;
}

class cEasingDemoEntityFactory
	: public GameBase::cEntityFactory
{
public:
	BREAKOUT_API cEasingDemoEntityFactory();
	BREAKOUT_API ~cEasingDemoEntityFactory();
	void VRegisterEntities();
};

#endif // EasingDemoEntityFactory_H__
