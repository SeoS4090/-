#include "UIObject.h"


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
}