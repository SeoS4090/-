#include "GameManager.h"
#include "Pawn.h"
#include "Look.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"
#include "BackGround.h"
#include "resource.h"
#include "Mecro.h"
using namespace std;
GameManager* GameManager::p_This = NULL;


GameManager::GameManager()
{
}


GameManager::~GameManager()
{
}

void GameManager::Init(HDC hdc, HINSTANCE g_hInst)
{

	/*배경 화면*/
	m_BackGround = new BackGround();
	m_BackGround->Init(hdc, g_hInst);

	/*검정 말*/
	for (int i = 0; i < 8; i++)
	{
		Pawn *pawn = new Pawn();
		pawn->Init(hdc, g_hInst, true);
		pawn->SetPt(i * 75, 75 * 6);

		BlackTeam.push_back(pawn);
	}
	for (int i = 0; i < 2; i++)
	{
		Look *look = new Look();
		look->Init(hdc, g_hInst, true);
		look->SetPt(i * 75*7, 75 * 7);

		BlackTeam.push_back(look);
	}
	for (int i = 0; i < 2; i++)
	{
		Knight *knight = new Knight();
		knight->Init(hdc, g_hInst, true);
		knight->SetPt(i * 75 * 5+75, 75 * 7);

		BlackTeam.push_back(knight);
	}
	for (int i = 0; i < 2; i++)
	{
		Bishop *bishop = new Bishop();
		bishop->Init(hdc, g_hInst, true);
		bishop->SetPt(i * 75 * 3 + 75 * 2, 75*7);

		BlackTeam.push_back(bishop);
	}

	Queen * black_queen = new Queen();
	black_queen->Init(hdc, g_hInst, true);
	black_queen->SetPt(75 * 3, 75 * 7);
	BlackTeam.push_back(black_queen);

	King * black_King = new King();
	black_King->Init(hdc, g_hInst, true);
	black_King->SetPt(75 * 4, 75 * 7);
	BlackTeam.push_back(black_King);

	/*흰색 말*/
	for (int i = 0; i < 8; i++)
	{
		Pawn *pawn = new Pawn();
		pawn->Init(hdc, g_hInst, false);
		pawn->SetPt(i * 75, 75);

		WhiteTeam.push_back(pawn);
	}
	for (int i = 0; i < 2; i++)
	{
		Look *look = new Look();
		look->Init(hdc, g_hInst, false);
		look->SetPt(i * 75 * 7, 0);

		WhiteTeam.push_back(look);
	}

	for (int i = 0; i < 2; i++)
	{
		Knight *knight = new Knight();
		knight->Init(hdc, g_hInst, false);
		knight->SetPt(i * 75 * 5 + 75, 0);

		WhiteTeam.push_back(knight);
	}
	for (int i = 0; i < 2; i++)
	{
		Bishop *bishop = new Bishop();
		bishop->Init(hdc, g_hInst, false);
		bishop->SetPt(i * 75 * 3 + 75*2, 0);

		WhiteTeam.push_back(bishop);
	}
	Queen * white_queen = new Queen();
	white_queen->Init(hdc, g_hInst, false);
	white_queen->SetPt(75 * 3, 0);
	WhiteTeam.push_back(white_queen);

	King * white_King = new King();
	white_King->Init(hdc, g_hInst, false);
	white_King->SetPt(75 * 4, 0);
	WhiteTeam.push_back(white_King);

	Turn = true;
	Game_State = GAME_STATE_BLACK;
	my_Piece = BlackTeam.end();
}

void GameManager::Draw(HDC hdc)
{
	m_BackGround->Draw(hdc);

	for (auto iter = BlackTeam.begin(); iter != BlackTeam.end(); iter++)
	{
		(*iter)->Draw(hdc);
	}

	for (auto iter = WhiteTeam.begin(); iter != WhiteTeam.end(); iter++)
	{
		(*iter)->Draw(hdc);
	}
}

void GameManager::Click(HDC hdc, HINSTANCE g_hInst,HWND hWnd,int x, int y)
{
	POINT pt;
	pt.x = x;
	pt.y = y;
	vector<BitMap *>::iterator iter;
	RECT rec;
	rec.left = int(pt.x / 75) *75;
	rec.top = int(pt.y / 75) *75;
	rec.right = rec.left + 75;
	rec.bottom = rec.top + 75;

	if (Turn)
	{
		for (iter = BlackTeam.begin(); iter != BlackTeam.end(); iter++)
		{
			if (PtInRect(&(*iter)->GetRect(), pt) && Game_State == GAME_STATE_BLACK)
			{
				(*iter)->Click(m_BackGround);
				Game_State = GAME_STATE_BLACK_CLICK;
				my_Piece = iter;
				break;
			}
			
			//검정테두리 클릭시 이동
			else if (m_BackGround->GetClickMap(pt.y/75,pt.x/75) == 1 &&PtInRect(&rec, pt))
			{
				Game_State = GAME_STATE_WHITE;
				m_BackGround->SetChessMap((*my_Piece)->GetRect().top / 75, (*my_Piece)->GetRect().left / 75, 0);
				(*my_Piece)->Click(m_BackGround);
				(*my_Piece)->SetPt(rec.left, rec.top);
				if (m_BackGround->GetChessMap(pt.y / 75, pt.x / 75) == WHITE_TEAM)
				{
					for (auto enumy = WhiteTeam.begin(); enumy != WhiteTeam.end(); enumy++)
					{
						if ((*enumy)->GetRect().left / 75 == pt.x / 75 && (*enumy)->GetRect().top / 75 == pt.y / 75)
						{
							if ((*enumy)->GetID() == "White King")
								if (MessageBox(hWnd, "Black Team Win", "Click", MB_OK))
								{
									Game_State = GAME_STATE_END;
									break;
								}
							SAFE_DELETE (*enumy);
							WhiteTeam.erase(enumy);
							
							break;
						}
					}
				}
				m_BackGround->SetChessMap(pt.y/75, pt.x/75,BLACK_TEAM);
				if ((*my_Piece)->GetID() == "Black Pawn" && (*my_Piece)->GetRect().top / 75 == 0)
				{
					Queen *black_Pawn_Queen = new Queen();
					black_Pawn_Queen->Init(hdc, g_hInst, true);
					black_Pawn_Queen->SetPt((*my_Piece)->GetRect().left, (*my_Piece)->GetRect().top);
					BlackTeam.push_back(black_Pawn_Queen);
					BlackTeam.erase(my_Piece);
				}

				m_BackGround->Clear();
				
				Turn = !Turn;
				break;
			}
			else if (Game_State == GAME_STATE_BLACK_CLICK)
			{
				m_BackGround->Clear();
				Game_State = GAME_STATE_BLACK;
			}
		}
		if (iter == BlackTeam.end())
			MessageBox(hWnd, "Please Click Black Team", "Click", MB_OK);
	}
	else
	{
		for (iter = WhiteTeam.begin(); iter != WhiteTeam.end(); iter++)
		{
			if (PtInRect(&(*iter)->GetRect(), pt) && Game_State == GAME_STATE_WHITE)
			{
				(*iter)->Click(m_BackGround);
				Game_State = GAME_STATE_WHITE_CLICK;
				my_Piece = iter;
				break;
			}
			else if (m_BackGround->GetClickMap(pt.y / 75, pt.x / 75) == 1 && PtInRect(&rec, pt))
			{
				Game_State = GAME_STATE_BLACK;
				m_BackGround->SetChessMap((*my_Piece)->GetRect().top / 75, (*my_Piece)->GetRect().left / 75, 0);
				(*my_Piece)->Click(m_BackGround);
				(*my_Piece)->SetPt(rec.left, rec.top);
				if (m_BackGround->GetChessMap(pt.y / 75, pt.x / 75) == BLACK_TEAM)
				{
					for (auto enumy = BlackTeam.begin(); enumy != BlackTeam.end(); enumy++)
					{
						if ((*enumy)->GetRect().left / 75 == pt.x / 75 && (*enumy)->GetRect().top / 75 == pt.y / 75)
						{
							if ((*enumy)->GetID() == "Black King")
								if (MessageBox(hWnd, "White Team Win", "Click", MB_OK))
								{
									Game_State = GAME_STATE_END;
									break;
								}
							SAFE_DELETE(*enumy);
							BlackTeam.erase(enumy);
							
							break;
						}
					}
				}

				m_BackGround->SetChessMap(pt.y / 75, pt.x / 75, WHITE_TEAM);

				if ((*my_Piece)->GetID() == "White Pawn" && (*my_Piece)->GetRect().top / 75 == 7)
				{
					Queen *white_Pawn_Queen = new Queen();
					white_Pawn_Queen->Init(hdc, g_hInst, false);
					white_Pawn_Queen->SetPt((*my_Piece)->GetRect().left, (*my_Piece)->GetRect().top);
					WhiteTeam.push_back(white_Pawn_Queen);
					WhiteTeam.erase(my_Piece);
				}
				m_BackGround->Clear();

				
				Turn = !Turn;
				break;
			}
			else if (Game_State == GAME_STATE_WHITE_CLICK)
			{
				m_BackGround->Clear();
				Game_State = GAME_STATE_WHITE;
			}
		}
		if(iter == WhiteTeam.end())
			MessageBox(hWnd, "Please Click White Team", "Click", MB_OK);
	}
}

void GameManager::Release()
{
	for (auto iter = BlackTeam.begin(); iter != BlackTeam.end(); iter++)
	{
		SAFE_DELETE(*iter);
	}
	for (auto iter = WhiteTeam.begin(); iter != WhiteTeam.end(); iter++)
	{
		SAFE_DELETE(*iter);
	}
}