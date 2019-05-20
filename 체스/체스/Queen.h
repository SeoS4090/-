#pragma once
#include<Windows.h>
#include"BitMap.h"
class Queen : public BitMap
{
private:
	bool m_bTeam;
public:
	Queen();
	~Queen();
	void Init(HDC hdc, HINSTANCE g_hInst, bool Team);
	void Click(BackGround *Bg);
};

