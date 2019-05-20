#pragma once
#include <Windows.h>
#include "BitMap.h"

class BackGround;

class Pawn :public BitMap
{
private:
	bool m_bTeam;
	bool First;
public:
	Pawn();
	~Pawn();
	void Init(HDC hdc, HINSTANCE g_hInst, bool Team);
	void Click(BackGround *Bg);
	string GetID();
};