#pragma once
#include <Windows.h>
#include "BitMap.h"
class Bishop : public BitMap
{
private :
	bool m_bTeam;
public:
	Bishop();
	~Bishop();
	void Init(HDC hdc, HINSTANCE g_hInst, bool Team);
	void Click(BackGround *Bg);

};

