#include "stdafx.h"
#include "WindowControl.h"
#include "Sprite.hxx"
#include "DxBase.hxx"
#include "XMLNode.hxx"

using namespace Graphics;
using namespace Utilities;
using namespace Base;
using namespace std;

cHashedString cWindowControl::m_Name = cHashedString("windowcontrol");

//  *******************************************************************************************************************
cWindowControl::cWindowControl()
	: m_eWindowType(cWindowControlDef::WT_DESKTOP)
{
}

//  *******************************************************************************************************************
cWindowControl::~cWindowControl()
{
	VCleanup();
}

//  *******************************************************************************************************************
cWindowControl::cWindowControl(const cWindowControl& other)
	: cBaseControl(other)
	, m_eWindowType(other.m_eWindowType)
{
	for(auto iter = other.m_pChildControls.begin(); iter != other.m_pChildControls.end(); iter++)
	{
		shared_ptr<IBaseControl> pTest = (*iter);
		shared_ptr<IBaseControl> pControl = pTest->VDuplicate();
		VAddChildControl(pControl, m_pUIRootControl);
	}
}

//  *******************************************************************************************************************
void cWindowControl::VInitialize(const shared_ptr<Utilities::IXMLNode const> pXMLNode)
{
	if (pXMLNode == NULL)
	{
		return;
	}

	m_eWindowType = cWindowControlDef::WT_STANDARD;

	cBaseControl::VInitialize(pXMLNode);
}

//  *******************************************************************************************************************
shared_ptr<IBaseControl> cWindowControl::VDuplicate()
{
	return std::shared_ptr<IBaseControl>(DEBUG_NEW cWindowControl(*this));
}

//  *******************************************************************************************************************
void cWindowControl::Initialize(const cWindowControlDef& def)
{
	m_bAllowMovingControls = def.bAllowMovingControls;
	m_eWindowType = def.wType;
	if (m_eWindowType != cWindowControlDef::WT_DESKTOP && !def.strBGImageFile.IsEmpty())
	{
		m_pBGSprite = ISprite::CreateSprite();
		m_pBGSprite->VInitialize(def.strBGImageFile);
	}
	cBaseControl::Initialize(def);
}

//  *******************************************************************************************************************
bool cWindowControl::VOnLeftMouseButtonUp(const int X, const int Y)
{
	if (m_eWindowType != cWindowControlDef::WT_DESKTOP)
	{
		return cBaseControl::VOnLeftMouseButtonUp(X, Y);
	}
	return false;
}

//  *******************************************************************************************************************
bool cWindowControl::VOnLeftMouseButtonDown(const int X, const int Y)
{
	if (m_eWindowType != cWindowControlDef::WT_DESKTOP)
	{
		return cBaseControl::VOnLeftMouseButtonDown(X, Y);
	}
	return false;
}

//  *******************************************************************************************************************
bool cWindowControl::VOnMouseMove(const int X, const int Y)
{
	if (m_eWindowType != cWindowControlDef::WT_DESKTOP)
	{
		return cBaseControl::VOnMouseMove(X, Y);
	}
	return false;
}

//  *******************************************************************************************************************
void cWindowControl::VSetAbsolutePosition()
{
	cBaseControl::VSetAbsolutePosition();

	for(auto iter = m_pChildControls.begin(); iter != m_pChildControls.end(); iter++)
	{
		(*iter)->VSetAbsolutePosition();
	}
}

//  *******************************************************************************************************************
void cWindowControl::VAddChildControl(shared_ptr<IBaseControl> pChildControl, weak_ptr<IBaseControl> pUiRootWindowControl)
{
	shared_ptr<cBaseControl> pControl = static_pointer_cast<cBaseControl>(pChildControl);
	if (pControl)
	{
		pControl->SetParentControl(this);
		pControl->SetUIRootControl(pUiRootWindowControl);
		pControl->VSetAbsolutePosition();
		m_pChildControls.push_back(pControl);
	}
}

//  *******************************************************************************************************************
void cWindowControl::VRemoveAllChildren()
{
	if (m_bFocus)
	{
		m_pFocusControl = this;
	}
	for(auto iter = m_pChildControls.begin(); iter != m_pChildControls.end(); ++iter)
	{
		(*iter)->VDeRegisterAllCallBacks();
	}
	m_pChildControls.clear();
}

//  *******************************************************************************************************************
void cWindowControl::VRender(const ICamera* const pCamera)
{
	cBaseControl::VRender(pCamera);

	for(auto iter = m_pChildControls.rbegin(); iter != m_pChildControls.rend(); iter++)
	{
		(*iter)->VRender(pCamera);
	}
}

//  *******************************************************************************************************************
const shared_ptr<IBaseControl> cWindowControl::VFindChildControl(const cString & controlName)
{
	UINT64 checksum = cHashedString::CalculateChecksum(cStringUtilities::GetInLowerCase(controlName));
	return VFindChildControl(checksum);
}

//  *******************************************************************************************************************
const shared_ptr<IBaseControl> cWindowControl::VFindChildControl(const UINT64 hashedID)
{
	ControlList::const_iterator iter;
	for(iter = m_pChildControls.begin(); iter != m_pChildControls.end(); ++iter)
	{
		if ((*iter).get()->VGetControlNameHash() == hashedID)
		{
			break;
		}
	}
	SP_ASSERT(iter != m_pChildControls.end())(hashedID).SetCustomMessage("Could not find Child control in Base Control");
	if (iter == m_pChildControls.end())
	{
		return NULL;
	}
	else
	{
		return (*iter);
	}
}

//  *******************************************************************************************************************
const shared_ptr<IBaseControl> cWindowControl::VGetChildControlAtIndex(const int index)
{
	SP_ASSERT(index >= 0 && index < m_pChildControls.size());
	if (index < 0 || index >= m_pChildControls.size())
	{
		return NULL;
	}
	auto iter = m_pChildControls.begin();
	int i = 0;
	while(i < index)
	{
		iter++;
		i++;
	}
	return (*iter);
}

//  *******************************************************************************************************************
void cWindowControl::VMoveToFront(const IBaseControl* const pControl)
{
	auto iter = GetChildControlIterator(pControl);
	if (iter != m_pChildControls.end() && iter != m_pChildControls.begin())
	{
		m_pChildControls.splice(m_pChildControls.begin(), m_pChildControls, iter);
	}
}

//  *******************************************************************************************************************
void cWindowControl::VRemoveChildControl(const Base::cString& controlName)
{
	ControlList::iterator iter;
	UINT64 checksum = cHashedString::CalculateChecksum(cStringUtilities::GetInLowerCase(controlName));
	for(iter = m_pChildControls.begin(); iter != m_pChildControls.end(); ++iter)
	{
		if ((*iter).get()->VGetControlNameHash() == checksum)
		{
			break;
		}
	}
	SP_ASSERT(iter != m_pChildControls.end())(controlName).SetCustomMessage("Could not find Child control in Base Control");

	if (iter != m_pChildControls.end())
	{
		if ((*iter).get()->GetFocus())
		{
			m_pFocusControl = this;
		}
		m_pChildControls.erase(iter);
	}
}

//  *******************************************************************************************************************
bool cWindowControl::VPostToAll(const Base::AppMsg& msg)
{
	for(auto iter = m_pChildControls.begin(); iter != m_pChildControls.end(); iter++)
	{
		if ((*iter)->VPostMsg(msg))
		{
			return true;
		}
	}
	return false;
}

//  *******************************************************************************************************************
void cWindowControl::VCleanup()
{
	cBaseControl::VCleanup();
	VRemoveAllChildren();
}

//  *******************************************************************************************************************
int cWindowControl::VGetNumberOfChildControls(bool recursive) const
{
	int numberOfChildren = 0;
	for(auto iter = m_pChildControls.begin(); iter != m_pChildControls.end(); ++iter)
	{
		numberOfChildren++;
		if (recursive)
		{
			numberOfChildren += (*iter)->VGetNumberOfChildControls(recursive);
		}
	}
	return numberOfChildren;
}

//  *******************************************************************************************************************
cWindowControl::ControlList::const_iterator cWindowControl::GetChildControlIterator(const IBaseControl* const pChildControl)
{
	ControlList::const_iterator iter;
	for(iter = m_pChildControls.begin(); iter != m_pChildControls.end(); ++iter)
	{
		if ((*iter).get() == pChildControl)
		{
			break;
		}
	}
	if (iter == m_pChildControls.end())
	{
		SP_LOG_CUSTOM(AssertLevels::LEVEL_ERROR, 2, "Could not find Child control in Base Control");
	}
	return iter;
}

//  *******************************************************************************************************************
IBaseControl * IBaseControl::CreateWindowControl(const cWindowControlDef& def)
{
	cWindowControl * pControl = DEBUG_NEW cWindowControl();
	pControl->Initialize(def);

	return pControl;
}
