//  *******************************************************************************************************************
//  BaseComponent version:  1.0   Ankur Sheel  date: 2013/04/02
//  *******************************************************************************************************************
//  purpose:
//  *******************************************************************************************************************
#include "stdafx.h"
#include "BaseComponent.h"

using namespace GameBase;
using namespace Base;

//  *******************************************************************************************************************
cBaseComponent::cBaseComponent()
	: m_Active(true)
{
}

//  *******************************************************************************************************************
cBaseComponent::~cBaseComponent()
{
}

//  *******************************************************************************************************************
cBaseComponent::cBaseComponent(const cBaseComponent & other)
	: m_pOwner(other.m_pOwner)
	, m_Active(other.m_Active)
{
}

//  *******************************************************************************************************************
cBaseComponent & cBaseComponent::operator=(const cBaseComponent & other)
{
	m_pOwner = other.m_pOwner;
	m_Active = other.m_Active;
	return *this;
}

//  *******************************************************************************************************************
void cBaseComponent::VOnAttached(shared_ptr<IBaseEntity> const pOwner)
{
	m_pOwner = pOwner;
}
