#pragma once
#include <Windows.h>
#include <string>
#include <functional>
typedef std::function<bool(int)> pBoolFunc;
using namespace std;
enum UITYPE
{
	LABEL,
	BUTTON
};

class UIObject
{
protected:
	string Name;
	RECT m_RecPos;
public:
	virtual void Draw(HDC hdc) = 0;
	virtual bool Update(POINT pt) = 0;
	virtual void Init(HDC hdc, string s, int x, int y) = 0;
	inline void UIObject::SetPos(int left, int top, int right, int bottom)
	{
		m_RecPos = { left, top, right, bottom };
	}
	virtual UITYPE GetType() = 0;
	virtual void Release() = 0;
	inline string GetString()
	{
		return Name;
	}
	inline POINT GetPosition()
	{
		POINT pt = { m_RecPos.left ,m_RecPos.top};
		return pt;
	}
};