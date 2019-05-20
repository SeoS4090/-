#pragma once
#include <Windows.h>
#include "resource.h"

class BitMap;

class ResManager
{
private:
	BitMap* m_arrImg[INFO_END];
	static ResManager* s_This;
public:
	static ResManager* GetInstance()
	{
		if (s_This == NULL)
		{
			s_This = new ResManager();
		}
		return s_This;
	}
	void Init(HDC hdc, HINSTANCE g_hInst);
	void Release();
	BitMap* GetImg(int x);
	ResManager();
	~ResManager();
};