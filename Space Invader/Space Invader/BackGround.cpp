#include "BackGround.h"
#include "ResManager.h"
#include "BitMap.h"
#include <string>
using namespace std;



void SetString(HDC hdc, int num, int x, int y)
{
	string m_sbonus;
	m_sbonus = "";

	
	int i = 4 - to_string(num).size();
	
	for (int j = 0; j < i; j++)
		m_sbonus.append("0");

	m_sbonus.append(to_string(num));


	char m_arrbonus[5];
	strcpy(m_arrbonus, m_sbonus.c_str());

	for (int i = 0; i < 4; i++)
	{
		switch (m_arrbonus[i])
		{
		case '1':
			ResManager::GetInstance()->GetImg(INT_1)->Draw(hdc, 20 * i + x, y);
			break;
		case '2':
			ResManager::GetInstance()->GetImg(INT_2)->Draw(hdc, 20 * i + x, y);
			break;
		case '3':
			ResManager::GetInstance()->GetImg(INT_3)->Draw(hdc, 20 * i + x, y);
			break;
		case '4':
			ResManager::GetInstance()->GetImg(INT_4)->Draw(hdc, 20 * i + x, y);
			break;
		case '5':
			ResManager::GetInstance()->GetImg(INT_5)->Draw(hdc, 20 * i + x, y);
			break;
		case '6':
			ResManager::GetInstance()->GetImg(INT_6)->Draw(hdc, 20 * i + x, y);
			break;
		case '7':
			ResManager::GetInstance()->GetImg(INT_7)->Draw(hdc, 20 * i + x, y);
			break;
		case '8':
			ResManager::GetInstance()->GetImg(INT_8)->Draw(hdc, 20 * i + x, y);
			break;
		case '9':
			ResManager::GetInstance()->GetImg(INT_9)->Draw(hdc, 20 * i + x, y);
			break;
		case '0':
			ResManager::GetInstance()->GetImg(INT_0)->Draw(hdc, 20 * i + x, y);
			break;
		}
	}
}

void BackGround::Init()
{
	m_Score = 0;
}

void BackGround::Draw(HDC hdc)
{
	ResManager::GetInstance()->GetImg(INFO_BACKGROUND)->Draw(hdc, 0, 0);
	SetString(hdc, m_Score, 70, 55);
	SetString(hdc, 2000, 250, 55);
}


BackGround::BackGround()
{
}


BackGround::~BackGround()
{
}
