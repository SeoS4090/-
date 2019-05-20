#pragma once
#include <Windows.h>

class BitMap
{
private:
	HBITMAP m_hBitmap;
	HBITMAP m_hOldbitmap;
	HDC m_MemDC;
	SIZE m_Size;
	BITMAP Bit_Info;
public:
	void Init(HDC hdc, HINSTANCE g_hInst, LPCSTR bitMap);
	void Draw(HDC hdc, int x, int y);
	BitMap();
	~BitMap();
};
