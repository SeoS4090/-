#pragma once
#include <Windows.h>
#include "UIObject.h"

class Label : public UIObject
{
private:

public:
	Label();
	~Label();

	virtual void Draw(HDC hdc);
	virtual bool Update(POINT pt);
	virtual UITYPE GetType();
	virtual void Init(HDC hdc, string s, int x, int y);
	void ScrollLabel();
	virtual void Release();
	RECT GetRect();
};