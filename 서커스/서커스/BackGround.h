#pragma once
#include <Windows.h>
#include "resource.h"
class BackGround
{
private:
	int m_arr[LENGTH];
	int m_Miter[LENGTH / 10];
	int m_Move;
	int Trun_Count;
	int m_icon;
	int SavePoint;
	int m_BoNus;
	int m_Count;
	int m_Score;
	int m_SaveScore;
	bool m_Die;
	bool m_End;
	HDC m_MiterDC;
	HBITMAP m_hBitmap;
	HBITMAP m_Oldbitmap;

public:
	void Init(HDC hdc);
	void Draw(HDC hdc);
	void SetMove(int move);
	bool Save(RECT rt);
	bool GetEnd()
	{
		return m_End;
	}
	void PlusScore(int score)
	{
		
		m_Score += score;

		if (m_Score < 0)
			m_Score = 0;
		
	}
	void SetLive()
	{
		m_Score = m_SaveScore;
		m_Die = false;
	}
	int GetSavePoint()
	{

		return SavePoint+50;
	}
	void Win();
	void Die();
	BackGround();
	~BackGround();
};

