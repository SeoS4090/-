#include "Queen.h"
#include "resource.h"
#include "BackGround.h"

Queen::Queen()
{
}


Queen::~Queen()
{
}

void Queen::Init(HDC hdc, HINSTANCE g_hInst, bool Team)
{
	m_bTeam = Team;
	if (m_bTeam)
		BitMap::Init(hdc, g_hInst, BLACK_QUEEN);
	else
		BitMap::Init(hdc, g_hInst, WHITE_QUEEN);
}

void Queen::Click(BackGround *Bg)
{
	if (m_bTeam)
	{
		for (int i = 1; i < 8; i++) //전진
		{
			if (Bg->GetChessMap(GetRect().top / 75 - i, GetRect().left / 75) == BLACK_TEAM)
				break;
			if (GetRect().top / 75 - i >= 0)
				Bg->SetClickMap(GetRect().top / 75 - i, GetRect().left / 75, CLICK);

			if (Bg->GetChessMap(GetRect().top / 75 - i, GetRect().left / 75) == WHITE_TEAM)
				break;
		}

		for (int i = 1; i < 8; i++) //후진
		{
			if (Bg->GetChessMap(GetRect().top / 75 + i, GetRect().left / 75) == BLACK_TEAM)
				break;
			if (GetRect().top / 75 + i < 8)
				Bg->SetClickMap(GetRect().top / 75 + i, GetRect().left / 75, CLICK);

			if (Bg->GetChessMap(GetRect().top / 75 + i, GetRect().left / 75) == WHITE_TEAM)
				break;
		}

		for (int i = 1; i < 8; i++) //좌측
		{
			if (Bg->GetChessMap(GetRect().top / 75, GetRect().left / 75 - i) == BLACK_TEAM)
				break;
			if (GetRect().left / 75 - i >= 0)
				Bg->SetClickMap(GetRect().top / 75, GetRect().left / 75 - i, CLICK);

			if (Bg->GetChessMap(GetRect().top / 75, GetRect().left / 75 - i) == WHITE_TEAM)
				break;

		}
		for (int i = 1; i < 8; i++) //우측
		{

			if (Bg->GetChessMap(GetRect().top / 75, GetRect().left / 75 + i) == BLACK_TEAM)
				break;
			if (GetRect().left / 75 + i < 8)
				Bg->SetClickMap(GetRect().top / 75, GetRect().left / 75 + i, CLICK);
			if (Bg->GetChessMap(GetRect().top / 75, GetRect().left / 75 + i) == WHITE_TEAM)
				break;
		}

		for (int i = 1; i < 8; i++)
		{
			if (GetRect().left / 75 + i < 8 && GetRect().top / 75 + i < 8)
			{
				if (Bg->GetChessMap(GetRect().top / 75 + i, GetRect().left / 75 + i) == BLACK_TEAM)
					break;

				Bg->SetClickMap(GetRect().top / 75 + i, GetRect().left / 75 + i, CLICK); //검정말 후진좌대각

				if (Bg->GetChessMap(GetRect().top / 75 + i, GetRect().left / 75 + i) == WHITE_TEAM)
					break;
			}
		}
		for (int i = 1; i < 8; i++)
		{
			if (GetRect().top / 75 - i >= 0 && GetRect().left / 75 - i >= 0)
			{
				if (Bg->GetChessMap(GetRect().top / 75 - i, GetRect().left / 75 - i) == BLACK_TEAM)
					break;

				Bg->SetClickMap(GetRect().top / 75 - i, GetRect().left / 75 - i, CLICK); //검정말 전진좌대각

				if (Bg->GetChessMap(GetRect().top / 75 - i, GetRect().left / 75 - i) == WHITE_TEAM)
					break;
			}
		}
		for (int i = 1; i < 8; i++)
		{
			if (GetRect().left / 75 - i >= 0 && GetRect().top / 75 + i < 8)
			{
				if (Bg->GetChessMap(GetRect().top / 75 + i, GetRect().left / 75 - i) == BLACK_TEAM)
					break;

				Bg->SetClickMap(GetRect().top / 75 + i, GetRect().left / 75 - i, CLICK); //검정말 후진우대각

				if (Bg->GetChessMap(GetRect().top / 75 + i, GetRect().left / 75 - i) == WHITE_TEAM)
					break;
			}
		}
		for (int i = 1; i < 8; i++)
		{
			if (GetRect().top / 75 - i >= 0 && GetRect().left / 75 + i < 8)
			{
				if (Bg->GetChessMap(GetRect().top / 75 - i, GetRect().left / 75 + i) == BLACK_TEAM)
					break;
				Bg->SetClickMap(GetRect().top / 75 - i, GetRect().left / 75 + i, CLICK); //검정말 전진우대각

				if (Bg->GetChessMap(GetRect().top / 75 - i, GetRect().left / 75 + i) == WHITE_TEAM)
					break;
			}
		}
	}
	else
	{
		for (int i = 1; i < 8; i++) //전진
		{
			if (Bg->GetChessMap(GetRect().top / 75 - i, GetRect().left / 75) == WHITE_TEAM)
				break;
			if (GetRect().top / 75 - i >= 0)
				Bg->SetClickMap(GetRect().top / 75 - i, GetRect().left / 75, CLICK);
			if (Bg->GetChessMap(GetRect().top / 75 - i, GetRect().left / 75) == BLACK_TEAM)
				break;
		}

		for (int i = 1; i < 8; i++) //후진
		{
			if (Bg->GetChessMap(GetRect().top / 75 + i, GetRect().left / 75) == WHITE_TEAM)
				break;
			if (GetRect().top / 75 + i < 8)
				Bg->SetClickMap(GetRect().top / 75 + i, GetRect().left / 75, CLICK);
			if (Bg->GetChessMap(GetRect().top / 75 + i, GetRect().left / 75) == BLACK_TEAM)
				break;
		}

		for (int i = 1; i < 8; i++) //좌측
		{
			if (Bg->GetChessMap(GetRect().top / 75, GetRect().left / 75 - i) == WHITE_TEAM)
				break;
			if (GetRect().left / 75 - i >= 0)
				Bg->SetClickMap(GetRect().top / 75, GetRect().left / 75 - i, CLICK);
			if (Bg->GetChessMap(GetRect().top / 75, GetRect().left / 75 - i) == BLACK_TEAM)
				break;

		}
		for (int i = 1; i < 8; i++) //우측
		{

			if (Bg->GetChessMap(GetRect().top / 75, GetRect().left / 75 + i) == WHITE_TEAM)
				break;
			if (GetRect().left / 75 + i < 8)
				Bg->SetClickMap(GetRect().top / 75, GetRect().left / 75 + i, CLICK);
			if (Bg->GetChessMap(GetRect().top / 75, GetRect().left / 75 + i) == BLACK_TEAM)
				break;
		}

		for (int i = 1; i < 8; i++)
		{
			if (GetRect().left / 75 + i < 8 && GetRect().top / 75 + i < 8)
			{
				if (Bg->GetChessMap(GetRect().top / 75 + i, GetRect().left / 75 + i) == WHITE_TEAM)
					break;

				Bg->SetClickMap(GetRect().top / 75 + i, GetRect().left / 75 + i, CLICK); //흰말 전진좌대각

				if (Bg->GetChessMap(GetRect().top / 75 + i, GetRect().left / 75 + i) == BLACK_TEAM)
					break;
			}
		}
		for (int i = 1; i < 8; i++)
		{
			if (GetRect().top / 75 - i >= 0 && GetRect().left / 75 - i >= 0)
			{
				if (Bg->GetChessMap(GetRect().top / 75 - i, GetRect().left / 75 - i) == WHITE_TEAM)
					break;

				Bg->SetClickMap(GetRect().top / 75 - i, GetRect().left / 75 - i, CLICK); //흰말 후진좌대각

				if (Bg->GetChessMap(GetRect().top / 75 - i, GetRect().left / 75 - i) == BLACK_TEAM)
					break;
			}
		}
		for (int i = 1; i < 8; i++)
		{
			if (GetRect().left / 75 - i >= 0 && GetRect().top / 75 + i < 8)
			{
				if (Bg->GetChessMap(GetRect().top / 75 + i, GetRect().left / 75 - i) == WHITE_TEAM)
					break;

				Bg->SetClickMap(GetRect().top / 75 + i, GetRect().left / 75 - i, CLICK); //흰말 전진우대각

				if (Bg->GetChessMap(GetRect().top / 75 + i, GetRect().left / 75 - i) == BLACK_TEAM)
					break;
			}
		}
		for (int i = 1; i < 8; i++)
		{
			if (GetRect().top / 75 - i >= 0 && GetRect().left / 75 + i < 8)
			{
				if (Bg->GetChessMap(GetRect().top / 75 - i, GetRect().left / 75 + i) == WHITE_TEAM)
					break;
				Bg->SetClickMap(GetRect().top / 75 - i, GetRect().left / 75 + i, CLICK); //흰말 후진우대각

				if (Bg->GetChessMap(GetRect().top / 75 - i, GetRect().left / 75 + i) == BLACK_TEAM)
					break;
			}
		}
	}
}