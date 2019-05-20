#include "ResManager.h"
#include "Mecro.h"
#include "BitMap.h"
ResManager* ResManager::s_This = NULL;

ResManager::ResManager()
{
}


ResManager::~ResManager()
{
}

void ResManager::Init(HDC hdc, HINSTANCE g_hInst)
{
	for (int i = 0; i < INFO_END; i++)
	{
		m_arrImg[i] = new BitMap();
	}

	m_arrImg[BLOCK00]->Init(hdc, g_hInst, "./RES/block00.bmp");

	m_arrImg[BLOCK01]->Init(hdc, g_hInst, "./RES/block01.bmp");
	m_arrImg[BLOCK02]->Init(hdc, g_hInst, "./RES/block02.bmp");
	m_arrImg[BLOCK03]->Init(hdc, g_hInst, "./RES/block03.bmp");
	m_arrImg[BLOCK04]->Init(hdc, g_hInst, "./RES/block04.bmp");
	m_arrImg[BLOCK05]->Init(hdc, g_hInst, "./RES/block05.bmp");
	m_arrImg[BLOCK06]->Init(hdc, g_hInst, "./RES/block06.bmp");
	m_arrImg[BLOCK07]->Init(hdc, g_hInst, "./RES/block07.bmp");

	m_arrImg[BLOCK08]->Init(hdc, g_hInst, "./RES/block08.bmp");
	m_arrImg[BLOCK09]->Init(hdc, g_hInst, "./RES/block09.bmp");
	m_arrImg[BLOCK10]->Init(hdc, g_hInst, "./RES/block10.bmp");

	m_arrImg[BLOCK11]->Init(hdc, g_hInst, "./RES/block11.bmp");
	m_arrImg[BLOCK12]->Init(hdc, g_hInst, "./RES/block12.bmp");
	m_arrImg[BLOCK13]->Init(hdc, g_hInst, "./RES/block13.bmp");
	m_arrImg[BLOCK14]->Init(hdc, g_hInst, "./RES/block14.bmp");

	m_arrImg[E_DOWN_00]->Init(hdc, g_hInst, "./RES/e_down_00.bmp");

	m_arrImg[E_DOWN_01]->Init(hdc, g_hInst, "./RES/e_down_01.bmp");
	m_arrImg[E_DOWN_02]->Init(hdc, g_hInst, "./RES/e_down_02.bmp");
	m_arrImg[E_RIGHT_00]->Init(hdc, g_hInst, "./RES/e_right_00.bmp");
	m_arrImg[E_RIGHT_01]->Init(hdc, g_hInst, "./RES/e_right_01.bmp");
	m_arrImg[E_RIGHT_02]->Init(hdc, g_hInst, "./RES/e_right_02.bmp");
	m_arrImg[E_LEFT_00]->Init(hdc, g_hInst, "./RES/e_left_00.bmp");
	m_arrImg[E_LEFT_01]->Init(hdc, g_hInst, "./RES/e_left_01.bmp");

	m_arrImg[E_LEFT_02]->Init(hdc, g_hInst, "./RES/e_left_02.bmp");

	m_arrImg[E_UP_00]->Init(hdc, g_hInst, "./RES/e_up_00.bmp");

	m_arrImg[E_UP_01]->Init(hdc, g_hInst, "./RES/e_up_01.bmp");

	m_arrImg[E_UP_02]->Init(hdc, g_hInst, "./RES/e_up_02.bmp");
	m_arrImg[ENEMY_ICON]->Init(hdc, g_hInst, "./RES/enemy_icon.bmp");
	m_arrImg[EXPLOSION_00]->Init(hdc, g_hInst, "./RES/explosion_00.bmp");
	m_arrImg[EXPLOSION_01]->Init(hdc, g_hInst, "./RES/explosion_01.bmp");
	m_arrImg[EXPLOSION_02]->Init(hdc, g_hInst, "./RES/explosion_02.bmp");
	m_arrImg[EXPLOSION_03]->Init(hdc, g_hInst, "./RES/explosion_03.bmp");
	m_arrImg[EXPLOSION_04]->Init(hdc, g_hInst, "./RES/explosion_04.bmp");
	m_arrImg[MISSILE_00]->Init(hdc, g_hInst, "./RES/missile_00.bmp");
	m_arrImg[MISSILE_01]->Init(hdc, g_hInst, "./RES/missile_01.bmp");
	m_arrImg[MISSILE_02]->Init(hdc, g_hInst, "./RES/missile_02.bmp");
	m_arrImg[MISSILE_03]->Init(hdc, g_hInst, "./RES/missile_03.bmp");
	m_arrImg[PLAYER_ICON]->Init(hdc, g_hInst, "./RES/player_icon.bmp");
	m_arrImg[RESPONE00]->Init(hdc, g_hInst, "./RES/respone00.bmp");
	m_arrImg[RESPONE01]->Init(hdc, g_hInst, "./RES/respone01.bmp");
	m_arrImg[SHIELD_00]->Init(hdc, g_hInst, "./RES/shield_00.bmp");
	m_arrImg[SHIELD_01]->Init(hdc, g_hInst, "./RES/shield_01.bmp");
	m_arrImg[STAGE_ICON]->Init(hdc, g_hInst, "./RES/stage_icon.bmp");
	m_arrImg[TANK_DOWN_00]->Init(hdc, g_hInst, "./RES/tank_down_00.bmp");
	m_arrImg[TANK_DOWN_01]->Init(hdc, g_hInst, "./RES/tank_down_01.bmp");
	m_arrImg[TANK_LEFT_00]->Init(hdc, g_hInst, "./RES/tank_left_00.bmp");
	m_arrImg[TANK_LEFT_01]->Init(hdc, g_hInst, "./RES/tank_left_01.bmp");
	m_arrImg[TANK_RIGHT_00]->Init(hdc, g_hInst, "./RES/tank_right_00.bmp");
	m_arrImg[TANK_RIGHT_01]->Init(hdc, g_hInst, "./RES/tank_right_01.bmp");
	m_arrImg[TANK_UP_00]->Init(hdc, g_hInst, "./RES/tank_up_00.bmp");
	m_arrImg[TANK_UP_01]->Init(hdc, g_hInst, "./RES/tank_up_01.bmp");
	m_arrImg[BACKGROUND]->Init(hdc, g_hInst, "./RES/background.bmp");

	m_arrImg[INT_1]->Init(hdc, g_hInst, "./RES/1.bmp");
	m_arrImg[INT_2]->Init(hdc, g_hInst, "./RES/2.bmp");
	m_arrImg[INT_3]->Init(hdc, g_hInst, "./RES/3.bmp");
	m_arrImg[INT_4]->Init(hdc, g_hInst, "./RES/4.bmp");
	m_arrImg[INT_5]->Init(hdc, g_hInst, "./RES/5.bmp");
	m_arrImg[INT_6]->Init(hdc, g_hInst, "./RES/6.bmp");
	m_arrImg[INT_7]->Init(hdc, g_hInst, "./RES/7.bmp");
	m_arrImg[INT_8]->Init(hdc, g_hInst, "./RES/8.bmp");
	m_arrImg[INT_9]->Init(hdc, g_hInst, "./RES/9.bmp");
	m_arrImg[INT_0]->Init(hdc, g_hInst, "./RES/0.bmp");
}

void ResManager::Release()
{
	for (int i = 0; i < INFO_END; i++)
	{
		SAFE_DELETE(m_arrImg[i]);
	}
	SAFE_DELETE(s_This);
}

BitMap * ResManager::GetImg(int x)
{
	return m_arrImg[x];
}
