#pragma once
#include <Windows.h>
#include <vector>

class BitMap;
class BackGround;
class Pawn;
using namespace std;

enum GameState
{
	GAME_STATE_BLACK,
	GAME_STATE_BLACK_CLICK,
	GAME_STATE_WHITE,
	GAME_STATE_WHITE_CLICK,
	GAME_STATE_END
};
class GameManager
{
private:
	static GameManager* p_This;
	BackGround *m_BackGround;
	vector<BitMap *>BlackTeam;
	vector<BitMap *>WhiteTeam;
	vector<BitMap *>::iterator my_Piece;
	bool Turn;
	int Game_State;

	HDC m_hdc;
	HINSTANCE m_hInst;

public:
	static GameManager* GetInstance() {
		if (p_This == NULL)
			p_This = new GameManager();
		return p_This;
	}
	GameManager();
	~GameManager();

	void Init(HDC hdc, HINSTANCE g_hInst);
	void Draw(HDC hdc);
	void Click(HDC hdc, HINSTANCE g_hInst, HWND hWnd, int x, int y);
	void Release();
};

