//  *******************************************************************************************************************
//  BaseControl   version:  1.0   Ankur Sheel  date: 2011/07/21
//  *******************************************************************************************************************
//  purpose:
//  *******************************************************************************************************************
#include "stdafx.h"
#include "BaseControl.h"
#include "Sprite.hxx"
#include "Structures.h"
#include "GraphicUtils.h"
#include "XMLNode.hxx"
#include "Vector2.h"
#include "DxBase.hxx"

using namespace Graphics;
using namespace Utilities;
using namespace Base;
using namespace std;

//  *******************************************************************************************************************
cBaseControl::cBaseControl()
	: m_bVisible(true)
	, m_pBGSprite(NULL)
	, m_pParentControl(NULL)
	, m_vPosition(cVector2(-1.0f, -1.0f))
	, m_bFocus(false)
	, m_pFocusControl(NULL)
	, m_bIsLeftMouseDown(false)
	, m_bAllowMovingControls(false)
	, m_ConstrainInParent(true)
	, m_iMouseDownXPos(0)
	, m_iMouseDownYPos(0)
{
}

//  *******************************************************************************************************************
cBaseControl::cBaseControl(const cBaseControl & other)
	: m_bVisible(other.m_bVisible)
	, m_bFocus(false)
	, m_bAllowMovingControls(other.m_bAllowMovingControls)
	, m_bIsLeftMouseDown(false)
	, m_pFocusControl(other.m_pFocusControl)
	, m_pParentControl(other.m_pParentControl)
	, m_iMouseDownXPos(other.m_iMouseDownXPos)
	, m_iMouseDownYPos(other.m_iMouseDownYPos)
	, m_strControlName(other.m_strControlName)
	, m_ConstrainInParent(other.m_ConstrainInParent)
	, m_pBGSprite(NULL)
{
	if (other.m_pBGSprite != NULL)
	{
		m_pBGSprite = ISprite::CreateSprite();
		m_pBGSprite->VInitialize(other.m_pBGSprite->VGetTexture());
	}

	VSetPosition(other.m_vPosition);
	VSetSize(other.m_Size);
}

//  *******************************************************************************************************************
cBaseControl::~cBaseControl()
{
	VCleanup();
}

//  *******************************************************************************************************************
void cBaseControl::VInitialize(const shared_ptr<IXMLNode const> pXMLNode)
{
	if (pXMLNode == NULL)
	{
		return;
	}

	VSetControlName(pXMLNode->VGetChildValue("Name"));

	cString bgImageFileName = pXMLNode->VGetChildValue("BackgroundImage");
	if (!bgImageFileName.IsEmpty())
	{
		m_pBGSprite = ISprite::CreateSprite();
		m_pBGSprite->VInitialize(bgImageFileName);
	}

	m_ConstrainInParent = pXMLNode->VGetChildValueAsBool("ConstrainInParent", true);

	cVector2 ratio = GetUIRatio();
	cVector2 size = pXMLNode->VGetChildValueAsVector2("ScaleInPixels", cVector2(8.0f, 8.0f)) * ratio;
	VSetSize(size);

	InitializePosition(pXMLNode);

	bool visible = pXMLNode->VGetChildValueAsBool("Visible", true);
	VSetVisible(visible);
}

//  *******************************************************************************************************************
void cBaseControl::Initialize(const cBaseControlDef & def)
{
	VSetControlName(def.strControlName);
	VSetPosition(def.vPosition);
	VSetSize(def.vSize);
}

//  *******************************************************************************************************************
bool cBaseControl::VPostMsg(const AppMsg & msg)
{
	if (!m_bVisible)
	{
		return false;
	}

	switch(msg.message)
	{
	case WM_LBUTTONDOWN:
		if (IsCursorIntersect(LOWORD(msg.m_lParam), HIWORD(msg.m_lParam)))
		{
			if (!VPostToAll(msg))
			{
				VOnLeftMouseButtonDown(LOWORD(msg.m_lParam), HIWORD(msg.m_lParam));
				if (AllowMovingControl() && m_pParentControl)
				{
					m_pParentControl->VMoveToFront(this);
				}
				SetFocusControl(this);
			}
			return true;
		}
		break;

	case WM_LBUTTONUP:
		if (!VPostToAll(msg))
		{
			if (m_pFocusControl)
			{
				return m_pFocusControl->VOnLeftMouseButtonUp(LOWORD(msg.m_lParam), HIWORD(msg.m_lParam));
			}
		}
		break;

	case WM_MOUSEMOVE:
		if (!VPostToAll(msg))
		{
			if (m_pFocusControl)
			{
				return m_pFocusControl->VOnMouseMove(LOWORD(msg.m_lParam), HIWORD(msg.m_lParam));
			}
		}
		break;

	case WM_KEYUP:
		if (!VPostToAll(msg))
		{
			if (m_pFocusControl)
			{
				return m_pFocusControl->VOnKeyUp(msg.m_wParam);
			}
		}
		break;

	case WM_KEYDOWN:
		if (!VPostToAll(msg))
		{
			if (m_pFocusControl)
			{
				return m_pFocusControl->VOnKeyDown(msg.m_wParam);
			}
		}
		break;

	case WM_CHAR:
		if (!VPostToAll(msg))
		{
			if (m_pFocusControl)
			{
				return m_pFocusControl->VOnCharPress(msg.m_wParam);
			}
		}
		break;
	}
	return false;
}

//  *******************************************************************************************************************
void cBaseControl::VSetPosition(const cVector2 & vPosition)
{
	if (m_vPosition != vPosition)
	{
		m_vPosition = vPosition;
		VSetAbsolutePosition();
	}
}

//  *******************************************************************************************************************
void cBaseControl::VSetSize(const cVector2 & vSize)
{
	if (m_Size != vSize)
	{
		m_Size = vSize;

		if (m_pBGSprite)
		{
			m_pBGSprite->VSetSize(m_Size);
		}
	}
}

//  *******************************************************************************************************************
void cBaseControl::VRegisterCallBack(const UIEVENTTYPE::ENUM eventType, UIEventCallBackFn fnCallback)
{
	m_CallbackMap.insert(std::make_pair(eventType, fnCallback));
}

//  *******************************************************************************************************************
void cBaseControl::VDeregisterCallBack(const UIEVENTTYPE::ENUM eventType)
{
	m_CallbackMap.erase(eventType);
}

//  *******************************************************************************************************************
void cBaseControl::VDeRegisterAllCallBacks()
{
	m_CallbackMap.clear();
}

//  *******************************************************************************************************************
void cBaseControl::VSetText(const Base::cString & strText)
{
	SP_ASSERT(false).SetCustomMessage("This function should be implemented in a child class.");
}

//  *******************************************************************************************************************
bool cBaseControl::VOnLeftMouseButtonUp(const int X, const int Y)
{
	if (AllowMovingControl() && m_bIsLeftMouseDown)
	{
		SP_LOG(2, cStringUtilities::MakeFormatted("New Position - X : %f , Y : %f", m_vPosition.x, m_vPosition.y));
	}

	m_bIsLeftMouseDown = false;
	return true;
}

//  *******************************************************************************************************************
bool cBaseControl::VOnLeftMouseButtonDown(const int X, const int Y)
{
	m_iMouseDownXPos = X - static_cast<int>(m_vControlAbsolutePosition.x);
	m_iMouseDownYPos = Y - static_cast<int>(m_vControlAbsolutePosition.y);
	m_bIsLeftMouseDown = true;
	return true;
}

//  *******************************************************************************************************************
void cBaseControl::VRender(const ICamera * const pCamera)
{
	if (!m_bVisible)
	{
		return;
	}

	if (m_pBGSprite)
	{
		m_pBGSprite->VRender(pCamera);
	}
}

//  *******************************************************************************************************************
bool cBaseControl::VOnKeyDown(const unsigned int iCharID)
{
	if (UIEventCallBackFn * pFn = GetCallbackFromMap(UIEVENTTYPE::KEYDOWN))
	{
		stUIEventCallbackParam param;
		param.uiCharId = iCharID;
		(*pFn)(param);
	}
	return false;
}

//  *******************************************************************************************************************
bool cBaseControl::VOnKeyUp(const unsigned int iCharID)
{
	return false;
}

//  *******************************************************************************************************************
bool cBaseControl::VOnCharPress(const unsigned int iCharID)
{
	return false;
}

//  *******************************************************************************************************************
bool cBaseControl::VOnMouseMove(const int X, const int Y)
{
	if (AllowMovingControl() && m_bIsLeftMouseDown)
	{
		float x = m_vPosition.x + (X - static_cast<int>(m_vControlAbsolutePosition.x)) - m_iMouseDownXPos;
		float y = m_vPosition.y + (Y - static_cast<int>(m_vControlAbsolutePosition.y)) - m_iMouseDownYPos;
		if (m_ConstrainInParent)
		{
			ConstrainChildControl(x, y);
		}
		VSetPosition(cVector2(x, y));
		return true;
	}
	return false;
}

//  *******************************************************************************************************************
void cBaseControl::VSetAbsolutePosition()
{
	if (m_ConstrainInParent)
	{
		ConstrainChildControl(m_vPosition.x, m_vPosition.y);
	}
	m_vControlAbsolutePosition = m_vPosition;
	if (m_pParentControl)
	{
		m_vControlAbsolutePosition += m_pParentControl->m_vControlAbsolutePosition;
	}
	if (m_pBGSprite)
	{
		m_pBGSprite->VSetPosition(m_vControlAbsolutePosition);
	}
}

//  *******************************************************************************************************************
void cBaseControl::VOnFocusChanged()
{
}

//  *******************************************************************************************************************
void cBaseControl::VSetVisible(bool bIsVisible)
{
	m_bVisible = bIsVisible;
}

//  *******************************************************************************************************************
void cBaseControl::VCleanup()
{
	m_CallbackMap.clear();
	SafeDelete(&m_pBGSprite);
}

//  *******************************************************************************************************************
const cString& cBaseControl::VGetControlName() const
{
	return m_strControlName.GetString();
}

//  *******************************************************************************************************************
void cBaseControl::VSetControlName(const cString & inControlName)
{
	m_strControlName = cHashedString(cStringUtilities::GetInLowerCase(inControlName));
}

//  *******************************************************************************************************************
cString cBaseControl::VToXml() const
{
	SP_ASSERT_ERROR(false).SetCustomMessage("This should be called only for window control");
	return cString::EmptyString;
}

//  *******************************************************************************************************************
void cBaseControl::VAddChildControl(shared_ptr<IBaseControl> pChildControl, weak_ptr<IBaseControl> pUiRootWindowControl)
{
	SP_ASSERT_ERROR(false).SetCustomMessage("This should be called only for window control");
}

//  *******************************************************************************************************************
void cBaseControl::VRemoveAllChildren()
{
	SP_ASSERT_ERROR(false).SetCustomMessage("This should be called only for window control");
}

//  *******************************************************************************************************************
void cBaseControl::VRemoveChildControl(const cString & strControlName)
{
	SP_ASSERT_ERROR(false).SetCustomMessage("This should be called only for window control");
}

//  *******************************************************************************************************************
const shared_ptr<IBaseControl> cBaseControl::VFindChildControl(const cString & strControlName)
{
	SP_ASSERT_ERROR(false).SetCustomMessage("This should be called only for window control");

	return NULL;
}

//  *******************************************************************************************************************
const shared_ptr<IBaseControl> cBaseControl::VFindChildControl(const UINT64 hashedID)
{
	SP_ASSERT_ERROR(false).SetCustomMessage("This should be called only for window control");

	return NULL;
}

//  *******************************************************************************************************************
const shared_ptr<IBaseControl> cBaseControl::VGetChildControlAtIndex(const int index)
{
	SP_ASSERT_ERROR(false).SetCustomMessage("This should be called only for window control");

	return NULL;
}

//  *******************************************************************************************************************
void cBaseControl::VMoveToFront(const IBaseControl * const pControl)
{
	SP_ASSERT_ERROR(false).SetCustomMessage("This should be called only for window control");
}

//  *******************************************************************************************************************
bool cBaseControl::VPostToAll(const AppMsg & msg)
{
	return false;
}

//  *******************************************************************************************************************
bool cBaseControl::IsCursorIntersect(const float fX, const float fY)
{
	if ((fX >= m_vControlAbsolutePosition.x)
			&& (fX <= m_vControlAbsolutePosition.x + VGetSize().x)
			&& (fY >= m_vControlAbsolutePosition.y)
			&& (fY <= m_vControlAbsolutePosition.y + VGetSize().y))
	{
		return true;
	}
	return false;
}

//  *******************************************************************************************************************
void cBaseControl::SetFocusControl(const cBaseControl * const pControl)
{
	if (!m_bFocus || m_pFocusControl != pControl)
	{
		if (m_pParentControl)
		{
			m_pParentControl->SetFocusControl(pControl);
		}
		else
		{
			if (m_pFocusControl)
			{
				m_pFocusControl->SetFocus(false);
			}
			m_pFocusControl = const_cast<cBaseControl *>(pControl);
		}
		SetFocus(true);
	}
}
//  *******************************************************************************************************************
void cBaseControl::SetFocus(const bool bFocus)
{
	m_bFocus = bFocus;
	VOnFocusChanged();
}

//  *******************************************************************************************************************
void cBaseControl::ConstrainChildControl(float & fX, float & fY)
{
	// constrain child control in parent control
	if (m_pParentControl)
	{
		if (fX < 0)
		{
			fX = 0;
		}
		if ((fX + m_Size.x) > m_pParentControl->VGetSize().x)
		{
			fX = m_pParentControl->VGetSize().x - m_Size.x;
		}
		if (fY < 0)
		{
			fY = 0;
		}
		if ((fY + m_Size.y) > m_pParentControl->VGetSize().y)
		{
			fY = m_pParentControl->VGetSize().y - m_Size.y;
		}
	}
}

//  *******************************************************************************************************************
bool cBaseControl::AllowMovingControl()
{
	if (m_pParentControl)
	{
		return m_pParentControl->AllowMovingControl();
	}
	return m_bAllowMovingControls;
}

//  *******************************************************************************************************************
UIEventCallBackFn * cBaseControl::GetCallbackFromMap(const UIEVENTTYPE::ENUM eventType)
{
	auto iter = m_CallbackMap.find(eventType);
	if (iter != m_CallbackMap.end())
	{
		return &(iter->second);
	}
	return NULL;
}

//  *******************************************************************************************************************
cVector2 cBaseControl::GetUIRatio() const
{
	cVector2 ratio = cVector2::One;
	shared_ptr<IBaseControl> pUiRootControl = MakeStrongPtr(m_pUIRootControl);

	if (pUiRootControl != NULL)
	{
		cVector2 originalSize = pUiRootControl->VGetSize();
		ratio = cVector2((IDXBase::GetInstance()->VGetScreenWidth() / originalSize.x), (IDXBase::GetInstance()->VGetScreenHeight() / originalSize.y));
	}
	return ratio;
}

//  *******************************************************************************************************************
void cBaseControl::InitializePosition(const shared_ptr<IXMLNode const> pXMLNode)
{
	cVector2 position;
	shared_ptr<IXMLNode> pNode(pXMLNode->VGetChild("Position"));
	if (pNode != NULL)
	{
		cVector2 parentPosition;
		cVector2 parentProportions = cVector2::One;

		if (m_pParentControl != NULL)
		{
			parentPosition = m_pParentControl->m_vPosition;
			parentProportions = m_pParentControl->VGetSize() * GetUIRatio();
		}
		position = parentPosition;
		bool relative = pNode->VGetNodeAttributeAsBool("relative", false);
		if (relative)
		{
			position += pXMLNode->VGetChildValueAsVector2("Position", cVector2::Zero) * parentProportions;
		}
		else
		{
			position += pXMLNode->VGetChildValueAsVector2("Position", cVector2::Zero) * GetUIRatio();
		}

		cString anchor = pNode->VGetNodeAttribute("anchor");
		if (anchor.IsEmpty() || anchor == "TL")
		{
			// do nothing - default
		}
		else if (anchor == "TC")
		{
			position.x += (parentProportions.x * 0.5f);
			position.x -= (m_Size.x * 0.5f);
		}
		else if (anchor == "TR")
		{
			position.x = parentPosition.x + parentProportions.x - (position.x - parentPosition.x);  // swap x coordinate
			position.x -= m_Size.x;  // put x back to top left
		}
		else if (anchor == "CL")
		{
			position.y += (parentProportions.y * 0.5f);
			position.y -= (m_Size.y * 0.5f);
		}
		else if (anchor == "Center")
		{
			// calculate center of parent element
			position += (parentProportions * 0.5f);
			position -= (m_Size * 0.5f);
		}
		else if (anchor == "CR")
		{
			position.x = parentPosition.x + parentProportions.x - (position.x - parentPosition.x);  // swap x coordinate
			position.x -= m_Size.x;  // put x back to top left
			position.y += (parentProportions.y * 0.5f);
			position.y -= (m_Size.y * 0.5f);
		}
		else if (anchor == "BL")
		{
			position.y = parentPosition.y + parentProportions.y - (position.y - parentPosition.y);  // swap y coordinate
			position.y -= m_Size.y;  // put y back to top left
		}
		else if (anchor == "BC")
		{
			position.x += (parentProportions.x * 0.5f);
			position.x -= (m_Size.x * 0.5f);

			position.y = parentPosition.y + parentProportions.y - (position.y - parentPosition.y);  // swap y coordinate
			position.y -= m_Size.y;  // put y back to top left
		}
		else if (anchor == "BR")
		{
			position.x = parentPosition.x + parentProportions.x - (position.x - parentPosition.x);  // swap x coordinate
			position.x -= m_Size.x;  // put x back to top left

			position.y = parentPosition.y + parentProportions.y - (position.y - parentPosition.y);  // swap y coordinate
			position.y -= m_Size.y;  // put y back to top left
		}
	}
	VSetPosition(position);
}

//  *******************************************************************************************************************
void cBaseControl::GenerateXml(shared_ptr<IXMLNode> pXmlNode) const
{
	if ((pXmlNode == NULL))
	{
		return;
	}

	shared_ptr<IXMLNode> pChildNode = MakeStrongPtr(pXmlNode->VAddElement("Name", m_strControlName.GetString()));
	cString bgImageFileName;
	 if (m_pBGSprite != NULL)
	 {
		 // bgImageFileName = m_pBGSprite->VGetTextureName();
	 }

	 pXmlNode->VAddElement("BackgroundImage", bgImageFileName);
	 pXmlNode->VAddElement("ConstrainInParent", m_ConstrainInParent ? "true" : "false");

	 cVector2 ratio = GetUIRatio();
	 pChildNode = MakeStrongPtr(pXmlNode->VAddElement("ScaleInPixels", ""));
	 pChildNode->VAddAttribute("relative", "false");
	 pChildNode->VAddAttribute("x", static_cast<int>(m_Size.x/ ratio.x));
	 pChildNode->VAddAttribute("y", static_cast<int>(m_Size.y/ ratio.y));

	 // InitializePosition(pXMLNode);

	pXmlNode->VAddElement("Visible", m_bVisible ? "true" : "false");
}
