#include "BitMap.h"

void BitMap::Init(HDC hdc, HINSTANCE g_hInst, LPCSTR bitMap)
{
	m_MemDC = CreateCompatibleDC(hdc);
	m_hBitmap = (HBITMAP)LoadImage(NULL, bitMap, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
	m_hOldbitmap = (HBITMAP)SelectObject(m_MemDC, m_hBitmap);

	GetObject(m_hBitmap, sizeof(Bit_Info), &Bit_Info);
	m_Size.cx = Bit_Info.bmWidth;
	m_Size.cy = Bit_Info.bmHeight;
}

void BitMap::Draw(HDC hdc, int x_start, int y_start)
{
	TransparentBlt(hdc, x_start, y_start, m_Size.cx, m_Size.cy, m_MemDC, 0, 0, Bit_Info.bmWidth, Bit_Info.bmHeight, RGB(255, 0, 255));
}

BitMap::BitMap()
{
}

BitMap::~BitMap()
{
}