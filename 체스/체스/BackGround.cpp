#include "BackGround.h"
#include "BitMap.h"
#include "resource.h"

BackGround::BackGround()
{
}


BackGround::~BackGround()
{
}

void BackGround::Init(HDC hdc, HINSTANCE g_hInst)
{
	BackGround1 = new BitMap();
	BackGround2 = new BitMap();
	Click = new BitMap();
	BackGround1->Init(hdc, g_hInst, BACK_GROUND1);
	BackGround2->Init(hdc, g_hInst, BACK_GROUND2);
	Click->Init(hdc, g_hInst, BACK_GROUNDCLICK);

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			Map[i][j] = (i + j) % 2;
			ClickMap[i][j] = EMPTY;
			ChessMap[i][j] = EMPTY;
		}
	}

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j< 2; j++)
			ChessMap[j][i] = WHITE_TEAM;
	}

	for (int i = 0; i < 8; i++)
	{
		for (int j = 6; j< 8; j++)
			ChessMap[j][i] = BLACK_TEAM;
	}
}

void BackGround::Draw(HDC hdc)
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (Map[i][j] == 1)
			{
				BackGround1->SetPt(i * 75, j * 75);
				BackGround1->Draw(hdc);
			}
			else
			{
				BackGround2->SetPt(i * 75, j * 75);
				BackGround2->Draw(hdc);
			}

			if (ClickMap[j][i] == CLICK)
			{
				Click->SetPt(i * 75, j * 75);
				Click->Draw(hdc);
			}
		}
	}
}

void BackGround::SetClickMap(int Height, int Width, int state)
{
	ClickMap[Height][Width] = state;
}
void BackGround::SetChessMap(int Height, int Width, int Team)
{
	ChessMap[Height][Width] = Team;
}

int BackGround::GetClickMap(int x, int y)
{
	return ClickMap[x][y];
}

int BackGround::GetChessMap(int x, int y)
{
	return ChessMap[x][y];
}

void BackGround::Clear()
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (ClickMap[i][j] == CLICK)
				ClickMap[i][j] = EMPTY;
		}
	}
}