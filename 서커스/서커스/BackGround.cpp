#include "BackGround.h"
#include "ResManager.h"
#include "BitMap.h"
#include <string>
using namespace std;

void BackGround::Init(HDC hdc)
{
	m_MiterDC = CreateCompatibleDC(hdc);
	m_hBitmap = CreateCompatibleBitmap(hdc, 170, 60);
	m_Oldbitmap = (HBITMAP)SelectObject(m_MiterDC, m_hBitmap);

	for (int i = 0; i < LENGTH; i++)
	{
		m_arr[i] = i % 4;
		if (m_arr[i] == 0)
			m_arr[i] = BACK_DECO;
		else
			m_arr[i] = BACK_NOMAL;
	}
	for (int i = 0; i < LENGTH / 10; i++)
	{
		m_Miter[i] = BACK_MITER;
	}
	m_Move = 0;
	Trun_Count = 0;
	m_icon = 4;
	SavePoint = 0;
	m_BoNus = 5000;
	m_Count = 0;
	m_Die = false;
	m_Score = 0;
	m_SaveScore = 0;
	m_End = false;

}

void SetString(HDC hdc, int num, int x, int y, bool score = false)
{
	string m_sbonus;
	

	if (!score)
		m_sbonus = to_string(num);
	else
	{
		int i = 6 - to_string(num).size();
		m_sbonus = "";
		for (int j = 0; j < i; j++)
			m_sbonus.append("0");
		m_sbonus.append(to_string(num));
	}
	char m_arrbonus[8];
	strcpy(m_arrbonus, m_sbonus.c_str());

	for (int i = 0; i < 8; i++)
	{
		switch (m_arrbonus[i])
		{
		case '1':
			ResManager::GetInstance()->GetImg(INT_1)->Draw(hdc, 28 * i + x, y);
			break;
		case '2':
			ResManager::GetInstance()->GetImg(INT_2)->Draw(hdc, 28 * i + x, y);
			break;
		case '3':
			ResManager::GetInstance()->GetImg(INT_3)->Draw(hdc, 28 * i + x, y);
			break;
		case '4':
			ResManager::GetInstance()->GetImg(INT_4)->Draw(hdc, 28 * i + x, y);
			break;
		case '5':
			ResManager::GetInstance()->GetImg(INT_5)->Draw(hdc, 28 * i + x, y);
			break;
		case '6':
			ResManager::GetInstance()->GetImg(INT_6)->Draw(hdc, 28 * i + x, y);
			break;
		case '7':
			ResManager::GetInstance()->GetImg(INT_7)->Draw(hdc, 28 * i + x, y);
			break;
		case '8':
			ResManager::GetInstance()->GetImg(INT_8)->Draw(hdc, 28 * i + x, y);
			break;
		case '9':
			ResManager::GetInstance()->GetImg(INT_9)->Draw(hdc, 28 * i + x, y);
			break;
		case '0':
			ResManager::GetInstance()->GetImg(INT_0)->Draw(hdc, 28 * i + x, y);
			break;
		}
	}
}

void BackGround::Draw(HDC hdc)
{
	if (m_Count > 5 && !m_Die && m_BoNus >= 0)
	{
		m_Count = 0;
		m_BoNus--;
	}
	m_Count++;

	ResManager::GetInstance()->GetImg(PANEL)->Draw(hdc, (1024 - 851) / 2, 0);
	SetString(hdc, m_BoNus, (1024 - 851) / 2 + 851 / 2 + 50, 50);
	SetString(hdc, m_Score, 200, 18,true);
	SetString(hdc, 20000, 500, 18);

	for (int i = m_icon; i > 0; i--)
	{
		ResManager::GetInstance()->GetImg(ICON)->Draw(hdc, (1024 - 851) / 2 + 851 - 30 - 34 * i, 50);
	}
	for (int i = 0; i < LENGTH; i++)
	{
		if (i * 66 - m_Move> -66 && i*66 - m_Move < 1024)
		{
			if (m_arr[i] == BACK_DECO)
				ResManager::GetInstance()->GetImg(BACKDECO)->Draw(hdc, i * 66 - m_Move, 100);
			else if (m_arr[i] == BACK_NOMAL)
				ResManager::GetInstance()->GetImg(BACKNOMAL)->Draw(hdc, i * 66 - m_Move, 100);
			else if (m_arr[i] == BACK_WIN)
				ResManager::GetInstance()->GetImg(BACKNOMAL2)->Draw(hdc, i * 66 - m_Move, 100);
		}
	}

	for (int i = 0; i < 16; i++)
		ResManager::GetInstance()->GetImg(BACK)->Draw(hdc, i * 66, 100 + 67);
	

	
	for (int i = 0; i < LENGTH / 10 -1; i++)
	{
		ResManager::GetInstance()->GetImg(MITER)->Draw(m_MiterDC, 0, 0);
		SetString(m_MiterDC, LENGTH - i * 10, 30, 15);
		TransparentBlt(hdc, (LENGTH * 66) / (LENGTH / 10)*i - m_Move, 100 + 183 + 66 + 10, 172 / 2, 30, m_MiterDC, 0, 0, 170, 60, SRCCOPY);
	}
}
bool BackGround::Save(RECT rt)
{
	RECT Testrt;
	RECT m_rt[LENGTH / 10 - 1];
	for (int i = 0; i < LENGTH / 10 - 1; i++)
	{
		m_rt[i].left = (LENGTH * 66) / (LENGTH / 10)*i - m_Move;
		m_rt[i].top = 0;
		m_rt[i].right = m_rt[i].left+10;
		m_rt[i].bottom = 100 + 183 + 66 + 10 + 30;
		if (IntersectRect(&Testrt, &rt, &m_rt[i]) != 0)
		{
			SavePoint = m_Move;
			m_SaveScore = m_Score;

			return true;
		}
	}
	return false;
}
void BackGround::SetMove(int move)
{
	if (move < LENGTH * 66)
	{
			m_Move = move;
	}
}

void BackGround::Win()
{
	if (Trun_Count > 6)
	{
		for (int i = 0; i < LENGTH; i++)
		{
			if (m_arr[i] == BACK_NOMAL)
				m_arr[i] = BACK_WIN;
			else if (m_arr[i] == BACK_WIN)
				m_arr[i] = BACK_NOMAL;
		}
		Trun_Count = 0;
	}
	else
		Trun_Count++;

	m_Die = true;
	if (m_BoNus > 0)
	{
		m_BoNus -= 10;
		m_Score += 10;
	}
	else if(m_BoNus != 0)
	{
		m_Score += m_BoNus;
		m_BoNus = 0;
	}
}

void BackGround::Die()
{
	m_icon--;
	m_Die = true;
	if (m_icon < 0)
		m_End = true;
	else
		m_End = false;
}

BackGround::BackGround()
{
}


BackGround::~BackGround()
{
}
