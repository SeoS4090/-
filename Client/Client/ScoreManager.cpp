#include "ScoreManager.h"
#include "Defines.h"
#include "ResourceManager.h"
#include <random>
#include "UIManager.h"
#include <math.h>

void ScoreManager::AddScore()
{
	SCORE_Info* m_Score = new SCORE_Info();
	srand(GetTickCount());
	int m_rand = rand() % 4;

	if (m_rand == 0 || m_rand == 1)
	{
		m_Score->pt.x = rand() % (GAME_WIDTH - HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/Score.bmp")->GetWidth());
		if (m_rand == 0)
		{
			m_Score->pt.y = GAME_HEIGHT_START;
			while (true)
			{
				m_Score->direct = SCORE_DIRECT(rand() % 8);
				if (m_Score->direct != UP && m_Score->direct != UPLEFT && m_Score->direct != UPRIGHT && m_Score->direct != LEFT && m_Score->direct != RIGHT)
					break;
			}
		}
		else
		{
			m_Score->pt.y = GAME_HEIGHT_END - HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/Score.bmp")->GetHeight();
			while (true)
			{
				m_Score->direct = SCORE_DIRECT(rand() % 8);
				if (m_Score->direct != DOWN && m_Score->direct != DOWNLEFT && m_Score->direct != DOWNRIGHT && m_Score->direct != LEFT && m_Score->direct != RIGHT)
					break;
			}
		}
	}
	else
	{
		m_Score->pt.y = (rand() % (GAME_HEIGHT_END - GAME_HEIGHT_START - HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/Score.bmp")->GetHeight())) + GAME_HEIGHT_START;

		if (m_rand == 2)
		{
			m_Score->pt.x = 1;

			while (true)
			{
				m_Score->direct = SCORE_DIRECT(rand() % 8);
				if (m_Score->direct != LEFT && m_Score->direct != UPLEFT && m_Score->direct != DOWNLEFT && m_Score->direct != UP && m_Score->direct != DOWN)
					break;
			}
		}
		else
		{
			m_Score->pt.x = GAME_WIDTH - HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/score.bmp")->GetWidth();
			while (true)
			{
				m_Score->direct = SCORE_DIRECT(rand() % 8);
				if (m_Score->direct != RIGHT && m_Score->direct != UPRIGHT && m_Score->direct != DOWNRIGHT && m_Score->direct != UP && m_Score->direct != DOWN)
					break;
			}
		}
	}

	m_Score->m_bFadeOut = false;

	for (auto iter = m_vecScore.begin(); iter != m_vecScore.end(); iter++)
	{
		HEngine::RECT Score_rt;
		Score_rt.left = (*iter)->pt.x;
		Score_rt.top = (*iter)->pt.y;
		Score_rt.right = (*iter)->pt.x + HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/Score.bmp")->GetWidth();
		Score_rt.bottom = (*iter)->pt.y + HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/Score.bmp")->GetHeight();

		HEngine::RECT m_Score_rt;
		m_Score_rt.left = (*iter)->pt.x;
		m_Score_rt.top = (*iter)->pt.y;
		m_Score_rt.right = (*iter)->pt.x + HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/Score.bmp")->GetWidth();
		m_Score_rt.bottom = (*iter)->pt.y + HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/Score.bmp")->GetHeight();

		if(Score_rt.isCollision(m_Score_rt) && !(*iter)->m_bFadeOut && (*iter)->direct == m_Score->direct)
		{
			SAFE_DELETE(m_Score);
			return;
		}
			
	}

	if (m_vecScore.size() >= 10)
	{
		vector<SCORE_Info*>::iterator iter;
		for (iter = m_vecScore.begin(); iter != m_vecScore.end(); iter++)
		{
			if ((*iter)->m_bFadeOut)
			{
				*iter = m_Score;
				break;
			}
		}
		if (iter == m_vecScore.end())
			SAFE_DELETE(m_Score);
	}
	else
		m_vecScore.push_back(m_Score);
}

void ScoreManager::Draw()
{
	for (auto iter = m_vecScore.begin(); iter != m_vecScore.end(); iter++)
	{
		if (!(*iter)->m_bFadeOut)
		{
			HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/Score.bmp")->Draw((*iter)->pt);
			HEngine::UIManager::GetInstance()->AddLabel(to_string(m_iScore), (*iter)->pt.x + 15, (*iter)->pt.y + 30, NULL, false);
		}
	}

	if(m_bFeverState)
	HEngine::ResourceManager::GetInstance()->GetBitmap(m_SscoreRes[(m_iScoreRes+1)%2])->Draw(25, 68);
	HEngine::ResourceManager::GetInstance()->GetBitmap(m_SscoreRes[m_iScoreRes])->Draw(25, 68, m_fFeverRateDraw);


}

void ScoreManager::ClearScore()
{
	for (auto iter = m_vecScore.begin(); iter != m_vecScore.end(); iter++)
	{
		(*iter)->m_bFadeOut = true;
	}
	m_iCombo = 1;
	m_fFeverRate = 0.0f;
}
int ScoreManager::GetScore()
{
	return m_iSumScore;
}
bool ScoreManager::Update(float fEtime, HEngine::RECT airplan)
{
	m_iScore = 100 * m_iCombo;
	if (m_fFeverRate >= 1.0f)
	{
		m_fFeverTime = 0.0f;
		m_fFeverRateDraw = 0;
		m_fFeverRate = 0.0f;
		m_bFeverState = true;
		m_iScoreRes++;
		m_iScoreRes = m_iScoreRes % 2;
	}
	else if (m_bFeverState)
	{
		m_fFeverTime += fEtime;
		if (m_fFeverTime >= 5.0f)
		{
			m_bFeverState = false;
			m_fFeverTime = 0;
			m_fFeverRate = 0.0f;
			m_iScoreRes = 0;
		}
	}
	if (m_fFeverRate > m_fFeverRateDraw)
	{
		m_fFeverRateDraw += fEtime;
		if (m_fFeverRate < m_fFeverRateDraw)
			m_fFeverRateDraw = m_fFeverRate;
	}

	else if (m_fFeverRate < m_fFeverRateDraw)
	{
		m_fFeverRateDraw -= fEtime;

		if (m_fFeverRate > m_fFeverRateDraw)
			m_fFeverRateDraw = m_fFeverRate;
	}
	m_fFeverRate += fEtime / 100;

	for (auto iter = m_vecScore.begin(); iter != m_vecScore.end(); iter++)
	{
		if ((*iter)->direct == LEFT || (*iter)->direct == UPLEFT || (*iter)->direct == DOWNLEFT)
			(*iter)->pt.x -= 70 * fEtime;
		else if ((*iter)->direct == RIGHT || (*iter)->direct == UPRIGHT || (*iter)->direct == DOWNRIGHT)
			(*iter)->pt.x += 70 * fEtime;

		if ((*iter)->direct == UP || (*iter)->direct == UPLEFT || (*iter)->direct == UPRIGHT)
			(*iter)->pt.y -= 70 * fEtime;
		else if ((*iter)->direct == DOWN || (*iter)->direct == DOWNLEFT || (*iter)->direct == DOWNRIGHT)
			(*iter)->pt.y += 70 * fEtime;

		if ((*iter)->pt.x < 0 - HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/Score.bmp")->GetWidth() || (*iter)->pt.x > GAME_WIDTH)
		{
			(*iter)->m_bFadeOut = true;
		}

		if ((*iter)->pt.y < GAME_HEIGHT_START - HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/Score.bmp")->GetHeight() || (*iter)->pt.y > GAME_HEIGHT_END )
		{
			(*iter)->m_bFadeOut = true;
		}

		HEngine::RECT Score_rt;
		Score_rt.left = (*iter)->pt.x;
		Score_rt.top = (*iter)->pt.y;
		Score_rt.right = (*iter)->pt.x + HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/Score.bmp")->GetWidth();
		Score_rt.bottom = (*iter)->pt.y + HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/Score.bmp")->GetHeight();
		if (Score_rt.isCollision(airplan) && !(*iter)->m_bFadeOut)
		{
			(*iter)->m_bFadeOut = true;
			m_iSumScore += 100 * m_iCombo;
			m_iCombo++;
			m_fFeverRate += 0.25f;
			
			return true;
		}
	}

	return false;
}

ScoreManager::ScoreManager()
{
	m_iSumScore = 0;
	m_fFeverRate = 0.0f;
	m_iScoreRes = 0;
	m_SscoreRes[0] = "./RES/fever00.bmp";
	m_SscoreRes[1] = "./RES/fever01.bmp";
	m_fFeverRateDraw = 0.0f;
	m_bFeverState = false;
	m_fFeverTime = 0.0f;
}


ScoreManager::~ScoreManager()
{
}
