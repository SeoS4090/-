#pragma once
#include <Windows.h>
#include <string>
using namespace std;
class BackGround;
class BitMap
{
private:
	HDC MemDC;
	HBITMAP MyBitmap;
	HBITMAP OldBitmap;
	SIZE bit_size;
	POINT pt;

public:
	BitMap();
	~BitMap();
	void Init(HDC hdc , HINSTANCE g_hInst, LPCSTR file);
	void Draw(HDC hdc);
	void SetPt(int x, int y);
	RECT GetRect();
	virtual void Click(BackGround *Bg);
	virtual string GetID();
};

