#include "Score.h"
#include "Defines.h"
#include "ResourceManager.h"
#include <random>


void Score::AddScore()
{
	SCORE_Info m_Score;
	srand(GetTickCount());
	int m_rand = rand() % 4;

	if (m_rand == 0 || m_rand == 1)
	{
		m_Score.pt.x = rand() % (GAME_WIDTH - HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/Score.bmp")->GetWidth());
		if (m_rand == 0)
		{
			m_Score.pt.y = GAME_HEIGHT_START;
			while (true)
			{
				m_Score.direct = SCORE_DIRECT(rand() % 8);
				if (m_Score.direct != UP && m_Score.direct != UPLEFT && m_Score.direct != UPRIGHT && m_Score.direct != LEFT && m_Score.direct != RIGHT)
					break;
			}

		}
		else
		{
			m_Score.pt.y = GAME_HEIGHT_END - HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/Score.bmp")->GetHeight();
			while (true)
			{
				m_Score.direct = SCORE_DIRECT(rand() % 8);
				if (m_Score.direct != DOWN && m_Score.direct != DOWNLEFT && m_Score.direct != DOWNRIGHT && m_Score.direct != LEFT && m_Score.direct != RIGHT)
					break;
			}
		}
	}
	else
	{
		m_Score.pt.y = (rand() % (GAME_HEIGHT_END - GAME_HEIGHT_START - HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/Score.bmp")->GetHeight())) + GAME_HEIGHT_START;

		if (m_rand == 2)
		{
			m_Score.pt.x = 1;

			while (true)
			{
				m_Score.direct = SCORE_DIRECT(rand() % 8);
				if (m_Score.direct != LEFT && m_Score.direct != UPLEFT && m_Score.direct != DOWNLEFT && m_Score.direct != UP && m_Score.direct != DOWN)
					break;
			}
		}
		else
		{
			m_Score.pt.x = GAME_WIDTH - HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/score.bmp")->GetWidth();
			while (true)
			{
				m_Score.direct = SCORE_DIRECT(rand() % 8);
				if (m_Score.direct != RIGHT && m_Score.direct != UPRIGHT && m_Score.direct != DOWNRIGHT && m_Score.direct != UP && m_Score.direct != DOWN)
					break;
			}
		}
	}

	m_Score.m_bFadeOut = false;

	if (m_vecScore.size() >= 50)
	{
		for (auto iter = m_vecScore.begin(); iter != m_vecScore.end(); iter++)
		{
			if ((*iter).m_bFadeOut)
			{
				(*iter) = m_Score;
				break;
			}
		}
	}
	else
		m_vecScore.push_back(m_Score);

}

void Score::Draw()
{
	for (auto iter = m_vecScore.begin(); iter != m_vecScore.end(); iter++)
	{
		if (!(*iter).m_bFadeOut)
			HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/Score.bmp")->Draw((*iter).pt);
	}
}

void Score::ClearScore()
{
	for (auto iter = m_vecScore.begin(); iter != m_vecScore.end(); iter++)
	{
		(*iter).m_bFadeOut = true;
	}
}

bool Score::Update(float fEtime, HEngine::RECT airplan)
{
	for (auto iter = m_vecScore.begin(); iter != m_vecScore.end(); iter++)
	{
		if ((*iter).direct == LEFT || (*iter).direct == UPLEFT || (*iter).direct == DOWNLEFT)
			(*iter).pt.x -= 50 * fEtime;
		else if ((*iter).direct == RIGHT || (*iter).direct == UPRIGHT || (*iter).direct == DOWNRIGHT)
			(*iter).pt.x += 50 * fEtime;

		if ((*iter).direct == UP || (*iter).direct == UPLEFT || (*iter).direct == UPRIGHT)
			(*iter).pt.y -= 50 * fEtime;
		else if ((*iter).direct == DOWN || (*iter).direct == DOWNLEFT || (*iter).direct == DOWNRIGHT)
			(*iter).pt.y += 50 * fEtime;

		if ((*iter).pt.x < 0 || (*iter).pt.x > GAME_WIDTH - HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/Score.bmp")->GetWidth())
		{
			(*iter).m_bFadeOut = true;
		}

		if ((*iter).pt.y < GAME_HEIGHT_START || (*iter).pt.y > GAME_HEIGHT_END - HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/Score.bmp")->GetHeight())
		{
			(*iter).m_bFadeOut = true;
		}

		HEngine::RECT Score_rt;
		Score_rt.left = (*iter).pt.x;
		Score_rt.top = (*iter).pt.y;
		Score_rt.right = (*iter).pt.x + HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/Score.bmp")->GetWidth();
		Score_rt.bottom = (*iter).pt.y + HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/Score.bmp")->GetHeight();
		if (Score_rt.isCollision(airplan) && !(*iter).m_bFadeOut)
		{
			return true;
		}
	}

	return false;
}

Score::Score()
{
}


Score::~Score()
{
}
