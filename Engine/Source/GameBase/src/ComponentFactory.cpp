#include "StdAfx.h"
#include "ComponentFactory.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "KeyboardHandlerComponent.h"
#include "PhysicsComponent.h"
#include "AIComponent.h"

using namespace GameBase;
using namespace Base;
using namespace Utilities;

cComponentFactory * cComponentFactory::m_pInstance = NULL;

//  *******************************************************************************************************************
cComponentFactory::cComponentFactory()
{
	if (m_pInstance == NULL)
		m_pInstance = this;
}

//  *******************************************************************************************************************
cComponentFactory::~cComponentFactory()
{
}

//  *******************************************************************************************************************
void cComponentFactory::VRegisterComponents()
{
	m_RegisteredComponents.Register<cTransformComponent>(cTransformComponent::GetName().GetHash());
	m_RegisteredComponents.Register<cSpriteComponent>(cSpriteComponent::GetName().GetHash());
	m_RegisteredComponents.Register<cKeyboardHandlerComponent>(cKeyboardHandlerComponent::GetName().GetHash());
	m_RegisteredComponents.Register<cPhysicsComponent>(cPhysicsComponent::GetName().GetHash());
	m_RegisteredComponents.Register<cAIComponent>(cAIComponent::GetName().GetHash());
}

//  *******************************************************************************************************************
shared_ptr<IBaseComponent> cComponentFactory::CreateComponent(const cHashedString & Type)
{
	shared_ptr<IBaseComponent> pComponent = m_RegisteredComponents.Create(Type.GetHash());
	return pComponent;
}

//  *******************************************************************************************************************
shared_ptr<IBaseComponent> cComponentFactory::GetDuplicateComponent(shared_ptr<IBaseComponent> const pComponent)
{
	shared_ptr<IBaseComponent> pNewComponent = m_RegisteredComponents.Duplicate(pComponent->VGetID(), pComponent);
	return pNewComponent;
}
