#include "Label.h"

Label::Label()
{
}

Label::~Label()
{
}

void Label::Draw(HDC hdc)
{
	DrawText(hdc, Name.c_str(), Name.length(), &m_RecPos, DT_CENTER);
}

bool Label::Update(POINT pt)
{
	return false;
}

UITYPE Label::GetType()
{
	return LABEL;
}
void Label::Init(HDC hdc, string s, int x, int y)
{
	SIZE size;
	GetTextExtentPoint32(hdc, s.c_str(), s.length(), &size);
	Name = s;
	SetPos(x, y, x + size.cx,y + size.cy);
}

void Label::ScrollLabel()
{/*
	for (auto iter = m_vecLabel.begin()+1; iter != m_vecLabel.end() -1; )
	{
		vector<Object *>::iterator iter2 = iter;
		iter++;
		(*iter2)->text = (*iter)->text;
	}
	m_vecLabel.pop_back();*/
}

void Label::Release()
{
}

RECT Label::GetRect()
{
	return m_RecPos;
}
