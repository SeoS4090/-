#include "King.h"
#include "resource.h"
#include "BackGround.h"

King::King()
{
}


King::~King()
{
}

void King::Init(HDC hdc, HINSTANCE g_hInst, bool Team)
{
	m_bTeam = Team;
	if (m_bTeam)
		BitMap::Init(hdc, g_hInst, BLACK_KING);
	else
		BitMap::Init(hdc, g_hInst, WHITE_KING);
}

void King::Click(BackGround *Bg)
{
	if (m_bTeam)
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				if(Bg->GetChessMap(GetRect().top / 75 - 1 + i, GetRect().left / 75 - 1 + j) != BLACK_TEAM)
					Bg->SetClickMap(GetRect().top / 75 - 1 + i, GetRect().left / 75 - 1 + j, CLICK);
			}
		}
	}
	else
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				if (Bg->GetChessMap(GetRect().top / 75 - 1 + i, GetRect().left / 75 - 1 + j) != WHITE_TEAM)
				Bg->SetClickMap(GetRect().top / 75 - 1 + i, GetRect().left / 75 - 1 + j, CLICK);
			}
		}
	}
}

string King::GetID()
{
	if (m_bTeam)
		return "Black King";
	else
		return "White King";
}