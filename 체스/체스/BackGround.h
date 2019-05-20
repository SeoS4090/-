#pragma once
#include <Windows.h>
class BitMap;
class BackGround
{
private:
	BitMap *BackGround1;
	BitMap *BackGround2;
	BitMap *Click;
	int Map[8][8];
	int ClickMap[8][8];
	int ChessMap[8][8];
public:
	BackGround();
	~BackGround();
	void Init(HDC hdc, HINSTANCE g_hInst);
	void Draw(HDC hdc);
	void SetClickMap(int x, int y, int state);
	void SetChessMap(int x, int y, int Team);
	int GetClickMap(int x, int y);
	int GetChessMap(int x, int y);
	void Clear();
};

