#pragma once
#include <Windows.h>
#include "BitMap.h"
class Look :public BitMap
{
private:
	bool m_bTeam;
public:
	Look();
	~Look();
	void Init(HDC hdc, HINSTANCE g_hInst, bool Team);
	void Click(BackGround *Bg);

};

