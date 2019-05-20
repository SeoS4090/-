#pragma once
#include <Windows.h>
#include "BitMap.h"

class King : public BitMap
{
private:
	bool m_bTeam;
public:
	King();
	~King();
	void Init(HDC hdc, HINSTANCE g_hInst, bool Team);
	void Click(BackGround *Bg);
	string GetID();
};

