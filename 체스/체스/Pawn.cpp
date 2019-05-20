#include "Pawn.h"
#include "resource.h"
#include "BackGround.h"

Pawn::Pawn()
{
}


Pawn::~Pawn()
{
}

void Pawn::Init(HDC hdc, HINSTANCE g_hInst, bool Team)
{
	m_bTeam = Team;
	if (m_bTeam)
		BitMap::Init(hdc, g_hInst, BLACK_PAWN);
	else
		BitMap::Init(hdc, g_hInst, WHITE_PAWN);

	First = true;
}

void Pawn::Click(BackGround *Bg)
{
	//ù�� ����
	if (m_bTeam == false && GetRect().top != 75)
		First = false;
	else if (m_bTeam == true && GetRect().top != 75 * 6)
		First = false;

	/*���� ��*/
	if (m_bTeam)
	{
		if (Bg->GetChessMap(GetRect().top / 75 - 1, GetRect().left / 75) != WHITE_TEAM && Bg->GetChessMap(GetRect().top / 75 - 1, GetRect().left / 75) == EMPTY)//�տ� ���� ���� ���� ������
		{
			//�׵θ� ����
			Bg->SetClickMap(GetRect().top / 75 - 1, GetRect().left / 75, CLICK);
			if (First && Bg->GetChessMap(GetRect().top / 75 - 2, GetRect().left / 75) == EMPTY) //ù��
				Bg->SetClickMap(GetRect().top / 75 - 2, GetRect().left / 75, CLICK);
		}
		if (Bg->GetChessMap(GetRect().top / 75 - 1, GetRect().left / 75 - 1) == WHITE_TEAM)//�´밢���� ���� ���� ���
		{
			Bg->SetClickMap(GetRect().top / 75 - 1, GetRect().left / 75 - 1, CLICK);
		}

		if (Bg->GetChessMap(GetRect().top / 75 - 1, GetRect().left / 75 + 1) == WHITE_TEAM)//��밢���� ���� ���� ���
		{
			Bg->SetClickMap(GetRect().top / 75 - 1, GetRect().left / 75 + 1, CLICK);
		}

	}
	/*��� ��*/
	else
	{
		if (Bg->GetChessMap(GetRect().top / 75 + 1, GetRect().left / 75) != BLACK_TEAM && Bg->GetChessMap(GetRect().top / 75 + 1, GetRect().left / 75) == EMPTY)//�տ� �������� ���� ���� ������
		{
			Bg->SetClickMap(GetRect().top / 75 + 1, GetRect().left / 75, CLICK);
			if (First && Bg->GetChessMap(GetRect().top / 75 + 2, GetRect().left / 75) == EMPTY)
				Bg->SetClickMap(GetRect().top / 75 + 2, GetRect().left / 75, CLICK);
		}
		if (Bg->GetChessMap(GetRect().top / 75 + 1, GetRect().left / 75 - CLICK) == BLACK_TEAM)//�´밢���� ���� ���� ���
		{
			Bg->SetClickMap(GetRect().top / 75 + 1, GetRect().left / 75 - CLICK, CLICK);
		}

		if (Bg->GetChessMap(GetRect().top / 75 + 1, GetRect().left / 75 + CLICK) == BLACK_TEAM)//��밢���� ���� ���� ���
		{
			Bg->SetClickMap(GetRect().top / 75 + 1, GetRect().left / 75 + CLICK, CLICK);
		}
	}
}

string Pawn::GetID()
{
	if (m_bTeam)
		return "Black Pawn";
	else
		return "White Pawn";
}