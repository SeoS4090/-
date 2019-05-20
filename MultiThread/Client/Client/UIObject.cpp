#include "UIObject.h"
#include "ResourceManager.h"

namespace HEngine
{
	void UIObject::SetPos(int left, int top, int right, int bottom)
	{
		m_RecPos.Set(left, top, right, bottom);
	}

	UIObject::UIObject()
	{
	}
	UIObject::~UIObject()
	{
	}
	string UIObject::GetString()
	{
		return UIName;
	}
	void UIObject::SetString(string s)
	{
		UIName = s;
		::SIZE size;
		GetTextExtentPoint32(HEngine::ResourceManager::GetInstance()->GetBackDC(), GetString().c_str(), GetString().length(), &size);
		SetPos(m_RecPos.left, m_RecPos.top, m_RecPos.left + size.cx, m_RecPos.top + size.cy);
	}
}