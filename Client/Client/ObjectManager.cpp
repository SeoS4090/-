#include "ObjectManager.h"
#include "Bullet.h"
#include "Star.h"
#include "Paper.h"
#include "UIManager.h"
#include "ResourceManager.h"
#include <random>
bool ObjectManager::AddBullet()
{
	int count = 0;
	for (auto iter = m_vecObject.begin(); iter != m_vecObject.end(); iter++)
	{
		if ((*iter)->GetType() == OBJECT_BULLET)
			count++;
	}
	if (count < BULLET_MAX_COUNT)
	{
		Bullet* m_bullet = new Bullet();
		m_vecObject.push_back(m_bullet);
		return true;
	}

	else
	{
		for (auto iter = m_vecObject.begin(); iter != m_vecObject.end(); iter++)
		{
			if ((*iter)->GetFade() && (*iter)->GetType() == OBJECT_BULLET)
			{
				auto del = *iter;
				del->Release();
				SAFE_DELETE(del);
				Bullet* m_bullet = new Bullet();
				*iter = m_bullet;
				
				return true;
			}
		}
	}

	return false;
}

bool ObjectManager::AddStar(bool Fever)
{
	int score = 100;
	int count = 0;

	Star* m_Score = new Star();
	
	HEngine::RECT newScore_rt;
	newScore_rt.left = m_Score->GetPosition().x;
	newScore_rt.top = m_Score->GetPosition().y;
	newScore_rt.right = m_Score->GetPosition().x + HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/Score.bmp")->GetWidth();
	newScore_rt.bottom = m_Score->GetPosition().y + HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/Score.bmp")->GetHeight();

	
	for (auto iter = m_vecObject.begin(); iter != m_vecObject.end(); iter++)
	{
		if ((*iter)->GetType() == OBJECT_STAR)
		{
			count++;
			if(!(*iter)->GetFade())
				score = (*iter)->GetScore();
		}
		if ((*iter)->GetType() != OBJECT_PAPER)
		{
			m_Score->SetCanMove();
		}

		HEngine::RECT Score_rt;
		Score_rt.left = (*iter)->GetPosition().x;
		Score_rt.top = (*iter)->GetPosition().y;
		Score_rt.right = (*iter)->GetPosition().x + HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/Score.bmp")->GetWidth();
		Score_rt.bottom = (*iter)->GetPosition().y + HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/Score.bmp")->GetHeight();

		if (Score_rt.isCollision(newScore_rt)&&!(*iter)->GetFade())
		{
			m_Score->Release();
			SAFE_DELETE(m_Score);
			return false;
		}
	}

	m_Score->SetScore(score);
	if (count < SCORE_MAX_COUNT || Fever)
	{
		m_vecObject.push_back(m_Score);
		return true;
	}

	else
	{
		for (auto iter = m_vecObject.begin(); iter != m_vecObject.end(); iter++)
		{
			if ((*iter)->GetFade() && (*iter)->GetType() == OBJECT_STAR)
			{
				auto del = *iter;
				del->Release();
				SAFE_DELETE(del);
				*iter = m_Score;
				return true;
			}
		}
		m_Score->Release();
		SAFE_DELETE(m_Score);
		return false;
	}
	return false;
}

bool ObjectManager::AddPaper(bool Fever)
{
	Paper* m_paper;
	if (m_vecObject.empty())
	{

		Paper* m_paper2 = new Paper();
		m_vecObject.push_back(m_paper2);

		m_paper = new Paper();
		m_vecObject.push_back(m_paper);

	}
	else
	{
		for (auto iter = m_vecObject.begin() + 1; iter != m_vecObject.end(); iter++)
		{
			(*iter)->Release();
			SAFE_DELETE(*iter);
		}
		m_vecObject.pop_back();
		m_vecObject.pop_back();
		m_paper = new Paper();
		m_vecObject.insert(m_vecObject.begin(),m_paper);
	}

	Star * star = new Star();
	if (!Fever)
		star->SetFade(bool(rand() % 5));

	if(!star->GetFade())
		m_icombo++;

	star->SetScore(m_icombo*100);
	
	HEngine::POINTF pt;
	pt.x = 195 + HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/score.bmp")->GetWidth() / 2;
	pt.y = 365 + HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/score.bmp")->GetHeight() / 2;
	star->SetPosition(pt);

	m_vecObject.push_back(star);

	
	srand(GetTickCount());

	

	return true;
}

ColliderType ObjectManager::Update(float fEtime, HEngine::RECT rect)
{
	for (auto iter = m_vecObject.begin(); iter != m_vecObject.end(); iter++)
	{

		if ((*iter)->Update(fEtime, rect))
		{
			if ((*iter)->GetType() == OBJECT_BULLET)
			{
				for (auto iter2 = m_vecObject.begin(); iter2 != m_vecObject.end(); iter2++)
				{
					(*iter2)->Release();
					SAFE_DELETE(*iter2);
				}
				m_vecObject.clear();

				return{ OBJECT_BULLET, true };
			}

			else if ((*iter)->GetType() == OBJECT_STAR)
			{
				for (auto scoreiter = m_vecObject.begin(); scoreiter != m_vecObject.end(); scoreiter++)
				{
					if ((*scoreiter)->GetType() == OBJECT_STAR)
						(*scoreiter)->SetScore((*scoreiter)->GetScore() + 100);
				}
				return{ OBJECT_STAR, true };
			}

			else if ((*iter)->GetType() == OBJECT_PAPER)
			{
				if (!m_vecObject.back()->GetFade())
				{
					m_score = m_vecObject.back()->GetScore();
				}
				return{ OBJECT_PAPER, true };
			}
		}
		else if ((*iter)->GetType() == OBJECT_PAPER)
		{
			Paper_Position m_position;
			HEngine::POINTF pt;
			pt.x = (*iter)->GetPosition().x + HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/score.bmp")->GetWidth() / 2;
			pt.y = (*iter)->GetPosition().y + HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/score.bmp")->GetHeight() / 2;
			(m_vecObject.back())->SetPosition(pt);

			m_score = 0;

		}
	}
	return{ OBJECT, false };
}

bool ObjectManager::Input(int key)
{
	if ((*(m_vecObject.begin() + 1))->GetPosition().x == 195 && (*(m_vecObject.begin() + 1))->GetPosition().y == 365)
	{
		bool m_bInput = (*(m_vecObject.begin() + 1))->Input(key);
		if (!m_bInput)
		{
			m_icombo = 0;
			(m_vecObject.back())->SetFade(true);
		}
		return m_bInput;
	}
}

void ObjectManager::Draw()
{
	for (auto iter = m_vecObject.begin(); iter != m_vecObject.end(); iter++)
	{
		(*iter)->Draw();
	}
}

void ObjectManager::ReleaseObject()
{
	for (auto iter = m_vecObject.begin(); iter != m_vecObject.end(); iter++)
	{
		(*iter)->Release();
		SAFE_DELETE(*iter);
	}
	m_vecObject.clear();
}

int ObjectManager::GetScore()
{
	return m_score;
}

ObjectManager::ObjectManager()
{
	m_score = 0;
	m_icombo = 0;
}


ObjectManager::~ObjectManager()
{
	
}
