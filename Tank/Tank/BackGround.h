#pragma once
#include <Windows.h>
#include "resource.h"
class BackGround
{
	int m_iEnemy;
	int m_iStage;
	int m_iLife;
	int m_iScore;
	GAME_STATE m_State;

public:
	void Init();
	void Draw(HDC hdc);
	bool SetStage();
	void GameEnd();
	void SetState(GAME_STATE gamestate);
	void PlayerDie()
	{
		m_iLife--;
	}
	void SetScore(int score)
	{
		if (m_iScore < score)
			m_iScore++;
	}
	GAME_STATE GetState()
	{
		return m_State;
	}
	int GetStage()
	{
		return m_iStage;
	}
	int GetEnemy()
	{
		return m_iEnemy;
	}
	void DecreaseEnemy()
	{
		m_iEnemy--;
	}
	BackGround();
	~BackGround();
};

