#include "Knight.h"
#include "resource.h"
#include "BackGround.h"

Knight::Knight()
{
}


Knight::~Knight()
{
}

void Knight::Init(HDC hdc, HINSTANCE g_hInst, bool Team)
{
	m_bTeam = Team;
	if (m_bTeam)
		BitMap::Init(hdc, g_hInst, BLACK_KNIGNT);
	else
		BitMap::Init(hdc, g_hInst, WHITE_KNIGNT);
}

void Knight::Click(BackGround *Bg)
{
	if (m_bTeam)
	{
		if (GetRect().top / 75 + 2 < 8)
		{
			if (GetRect().left / 75 - 1 >= 0 && Bg->GetChessMap(GetRect().top / 75 + 2, GetRect().left / 75 - 1) != BLACK_TEAM)
				Bg->SetClickMap(GetRect().top / 75 + 2, GetRect().left / 75 - 1, CLICK);
			if (GetRect().left / 75 + 1 < 8 && Bg->GetChessMap(GetRect().top / 75 + 2, GetRect().left / 75 + 1) != BLACK_TEAM)
				Bg->SetClickMap(GetRect().top / 75 + 2, GetRect().left / 75 + 1, CLICK);
		}

		if (GetRect().top / 75 + 1 < 8)
		{
			if (GetRect().left / 75 - 2 >= 0 && Bg->GetChessMap(GetRect().top / 75 + 1, GetRect().left / 75 - 2) != BLACK_TEAM)
			Bg->SetClickMap(GetRect().top / 75 + 1, GetRect().left / 75 - 2, CLICK);
			if (GetRect().left / 75 + 2 < 8 && Bg->GetChessMap(GetRect().top / 75 + 1, GetRect().left / 75 + 2) != BLACK_TEAM)
				Bg->SetClickMap(GetRect().top / 75 + 1, GetRect().left / 75 + 2, CLICK);
		}

		if (GetRect().top / 75 - 2 >= 0)
		{
			if (GetRect().left / 75 - 1 >= 0 && Bg->GetChessMap(GetRect().top / 75 - 2, GetRect().left / 75 - 1) != BLACK_TEAM)
				Bg->SetClickMap(GetRect().top / 75 - 2, GetRect().left / 75 - 1, CLICK);
			if (GetRect().left / 75 + 1 < 8 && Bg->GetChessMap(GetRect().top / 75 - 2, GetRect().left / 75 + 1) != BLACK_TEAM)
				Bg->SetClickMap(GetRect().top / 75 - 2, GetRect().left / 75 + 1, CLICK);
		}

		if (GetRect().top / 75 - 1 >= 0)
		{
			if (GetRect().left / 75 - 2 >= 0 && Bg->GetChessMap(GetRect().top / 75 - 1, GetRect().left / 75 - 2) != BLACK_TEAM)
				Bg->SetClickMap(GetRect().top / 75 - 1, GetRect().left / 75 - 2, CLICK);
			if (GetRect().left / 75 + 2 < 8 && Bg->GetChessMap(GetRect().top / 75 - 1, GetRect().left / 75 + 2) != BLACK_TEAM)
				Bg->SetClickMap(GetRect().top / 75 - 1, GetRect().left / 75 + 2, CLICK);
		}
	}

	else
	{
		if (GetRect().top / 75 + 2 < 8)
		{
			if (GetRect().left / 75 - 1 >= 0 && Bg->GetChessMap(GetRect().top / 75 + 2, GetRect().left / 75 - 1) != WHITE_TEAM)
				Bg->SetClickMap(GetRect().top / 75 + 2, GetRect().left / 75 - 1, CLICK);
			if (GetRect().left / 75 + 1 < 8 && Bg->GetChessMap(GetRect().top / 75 + 2, GetRect().left / 75 + 1) != WHITE_TEAM)
				Bg->SetClickMap(GetRect().top / 75 + 2, GetRect().left / 75 + 1, CLICK);
		}

		if (GetRect().top / 75 + 1 < 8)
		{
			if (GetRect().left / 75 - 2 >= 0 && Bg->GetChessMap(GetRect().top / 75 + 1, GetRect().left / 75 - 2) != WHITE_TEAM)
				Bg->SetClickMap(GetRect().top / 75 + 1, GetRect().left / 75 - 2, CLICK);
			if (GetRect().left / 75 + 2 < 8 && Bg->GetChessMap(GetRect().top / 75 + 1, GetRect().left / 75 + 2) != WHITE_TEAM)
				Bg->SetClickMap(GetRect().top / 75 + 1, GetRect().left / 75 + 2, CLICK);
		}

		if (GetRect().top / 75 - 2 >= 0)
		{
			if (GetRect().left / 75 - 1 >= 0 && Bg->GetChessMap(GetRect().top / 75 - 2, GetRect().left / 75 - 1) != WHITE_TEAM)
				Bg->SetClickMap(GetRect().top / 75 - 2, GetRect().left / 75 - 1, CLICK);
			if (GetRect().left / 75 + 1 < 8 && Bg->GetChessMap(GetRect().top / 75 - 2, GetRect().left / 75 + 1) != WHITE_TEAM)
				Bg->SetClickMap(GetRect().top / 75 - 2, GetRect().left / 75 + 1, CLICK);
		}

		if (GetRect().top / 75 - 1 >= 0)
		{
			if (GetRect().left / 75 - 2 >= 0 && Bg->GetChessMap(GetRect().top / 75 - 1, GetRect().left / 75 - 2) != WHITE_TEAM)
				Bg->SetClickMap(GetRect().top / 75 - 1, GetRect().left / 75 - 2, CLICK);
			if (GetRect().left / 75 + 2 < 8 && Bg->GetChessMap(GetRect().top / 75 - 1, GetRect().left / 75 + 2) != WHITE_TEAM)
				Bg->SetClickMap(GetRect().top / 75 - 1, GetRect().left / 75 + 2, CLICK);
		}
	}
}