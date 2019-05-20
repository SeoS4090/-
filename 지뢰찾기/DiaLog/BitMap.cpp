#include "BitMap.h"

BitMap::BitMap()
{
}


BitMap::~BitMap()
{
	DeleteObject(m_bBitmap);
	DeleteDC(MemDC);
}

void BitMap::Init(HDC hdc, HINSTANCE g_hInst, int x)
{
	MemDC = CreateCompatibleDC(hdc);
	m_bBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(x));
	m_bOldbitmap = (HBITMAP)SelectObject(MemDC, m_bBitmap);

	BITMAP Bit_Info;
	GetObject(m_bBitmap, sizeof(Bit_Info), &Bit_Info);
	m_siBitmap.cx = Bit_Info.bmWidth;
	m_siBitmap.cy = Bit_Info.bmHeight;
}

void BitMap::Draw(HDC hdc, int x, int y)
{
	BitBlt(hdc, x, y, m_siBitmap.cx, m_siBitmap.cy, MemDC, 0, 0, SRCCOPY);
}