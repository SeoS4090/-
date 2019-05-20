#pragma once
#include <Windows.h>
#include "BitMap.h"
class Knight :public BitMap
{
private :
	bool m_bTeam;
public:
	Knight();
	~Knight();
	void Init(HDC hdc, HINSTANCE g_hInst, bool Team);
	void Click(BackGround *Bg);

};

