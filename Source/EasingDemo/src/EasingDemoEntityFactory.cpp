// *****************************************************************************
//  BreakOutEntityFactory   version:  1.0   Ankur Sheel  date: 2013/05/06
// *****************************************************************************
//  purpose:
// *****************************************************************************

#include "StdAfx.h"
#include "EasingDemoEntityFactory.h"
#include "XMLNode.hxx"
#include "EntityManager.hxx"
#include "BaseComponent.hxx"
#include "Config.h"
#include "Ball.h"
#include "ComponentFactory.h"

using namespace GameBase;
using namespace Base;
using namespace Utilities;

// *****************************************************************************
cEasingDemoEntityFactory::cEasingDemoEntityFactory()
{
}

// *****************************************************************************
cEasingDemoEntityFactory::~cEasingDemoEntityFactory()
{
}

// *****************************************************************************
void cEasingDemoEntityFactory::VRegisterEntities()
{
  m_RegisteredEntities.Register<cBall>(cBall::m_EntityType.GetHash());
}
