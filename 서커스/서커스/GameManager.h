#pragma once
#include <Windows.h>

class BackGround;
class Player;
class Fire;
class GameManager
{
private:
	static GameManager* p_This;
	HDC BackDC;
	HBITMAP m_hBitmap;
	HBITMAP m_Oldbitmap;

	BackGround * m_BackGround;
	Fire * m_Fire;
	Player * m_Player;
	bool m_bjump;
	bool m_Die;
	bool m_End;
	int m_Count;



public:
	static GameManager* GetInstance()
	{
		if (p_This == NULL)
		{
			p_This = new GameManager();
		}
		return p_This;
	}
	void Init(HDC hdc, HINSTANCE g_hInst);
	void Draw(HDC hdc);
	void Update(HWND hWnd);
	void Move(int key);
	void Restart(HDC hdc);
	bool GetEnd()
	{
		return m_End;
	}
	void Release();
	GameManager();
	~GameManager();
};

