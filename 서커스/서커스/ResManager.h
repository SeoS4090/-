#pragma once
#include <Windows.h>


class BitMap;
enum IMG_RES
{
	BACK,
	BACKDECO,
	BACKNOMAL,
	BACKNOMAL2,
	CASH,
	DIE,
	END,
	ENEMY,
	ENEMY_1B,
	ENEMY_1F,
	ENEMY_B,
	ENEMY_F,
	ENEMY_L,
	ENEMY_L_B,
	ENEMY_L_F,
	ENEMY1,
	FRONT,
	FRONT2,
	ICON,
	MITER,
	PLAYER0,
	PLAYER1,
	PLAYER2,
	STAR,
	STAR1,
	STAR2,
	WIN,
	WIN2,
	PANEL,
	INT_1,
	INT_2,
	INT_3,
	INT_4,
	INT_5,
	INT_6,
	INT_7,
	INT_8,
	INT_9,
	INT_0,
	CASHSCORE,
	INFO_END
};
class ResManager
{
private:
	BitMap* m_arrImg[INFO_END];
	static ResManager* s_This;
public:
	static ResManager* GetInstance()
	{
		if (s_This == NULL)
		{
			s_This = new ResManager();
		}
		return s_This;
	}
	void Init(HDC hdc, HINSTANCE g_hInst);
	void Release();
	BitMap* GetImg(int x);
	ResManager();
	~ResManager();
};

