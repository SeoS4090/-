#pragma once
#include <Windows.h>

class BitMap
{
	HDC MemDC;
	HBITMAP m_bBitmap;
	HBITMAP m_bOldbitmap;
	SIZE m_siBitmap;
public:
	BitMap();
	~BitMap();
	void Init(HDC hdc, HINSTANCE g_hInst, int x);
	void Draw(HDC hdc, int x, int y);
};

