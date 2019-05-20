#include "Button.h"
#include "Mecro.h"
#include "Label.h"

Button::Button()
{
}
Button::~Button()
{
}
void Button::Init(HDC hdc , string s, int x, int y)
{
	btName = new Label();
	btName->Init(hdc, s, x + 3, y + 3);
	m_RecPos = btName->GetRect();
}


void Button::SetFunc(pBoolFunc Func, int arg)
{
	iarg = arg;
	btFunc = Func;
}

bool Button::Update(POINT pt)
{
	if (PtInRect(&m_RecPos, pt))
	{
		return btFunc(iarg);
	}
	return false;
}

UITYPE Button::GetType()
{
	return BUTTON;
}
void Button::Draw(HDC hdc)
{
	Rectangle(hdc, m_RecPos.left - 3, m_RecPos.top - 3, m_RecPos.right + 3, m_RecPos.bottom + 3);
	btName->Draw(hdc);
}

void Button::Release()
{
	SAFE_DELETE(btName);
}
