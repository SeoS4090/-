#pragma once
#include "UIObject.h"
#include <vector>
#include <Windows.h>

using namespace std;

class UIManager
{
private :
	static UIManager * pThis;
	vector<UIObject*> m_vecUI;
public:
	static UIManager * GetInstance()
	{
		if (pThis == NULL)
		{
			pThis = new UIManager();
		}
		return pThis;
	}
	bool AddLabel(HDC hdc, string s, POINT pt);
	void AddButton(HDC hdc, string s, POINT pt, pBoolFunc Func, int arg);
	bool Update(POINT pt);
	void Draw(HDC hdc);
	void Release();
	void Destroy();
};