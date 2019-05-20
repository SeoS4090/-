#include "BitMap.h"
#include "BackGround.h"


BitMap::BitMap()
{
}


BitMap::~BitMap()
{
}

void BitMap::Init(HDC hdc, HINSTANCE g_hInst, LPCSTR filename)
{
	MemDC = CreateCompatibleDC(hdc);
	MyBitmap = (HBITMAP)LoadImage(NULL, filename, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
	OldBitmap = (HBITMAP)SelectObject(MemDC, MyBitmap);
}

void BitMap::Draw(HDC hdc)
{
	TransparentBlt(hdc, pt.x, pt.y, 75,  75, MemDC, 0, 0, 125, 125, RGB(255, 0, 255));
}

void BitMap::SetPt(int x, int y)
{
	pt.x = x;
	pt.y = y;
}

RECT BitMap::GetRect()
{
	RECT Rect;
	Rect.left = pt.x;
	Rect.top = pt.y;
	Rect.right = pt.x + 75;
	Rect.bottom = pt.y + 75;
	return Rect;
}

void BitMap::Click(BackGround* Bg)
{

}

string BitMap::GetID()
{
	return "BitMap";
}