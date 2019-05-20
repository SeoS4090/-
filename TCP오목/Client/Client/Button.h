#pragma once
#include <Windows.h>
#include "UIObject.h"
#include "Label.h"


class Button : public UIObject
{
private:
	Label*	btName;
	pBoolFunc btFunc;
	int iarg;
public:
	Button();
	~Button();
	virtual void Init(HDC hdc, string s, int x, int y);

	void SetFunc(pBoolFunc Func, int arg);
	virtual bool Update(POINT pt);
	UITYPE GetType();
	virtual void Draw(HDC hdc);
	virtual void Release();
};