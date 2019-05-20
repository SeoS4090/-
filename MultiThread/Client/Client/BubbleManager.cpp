#include "BubbleManager.h"
#include "TimeManager.h"
#include "MapManager.h"
#include "InputManager.h"
bool BubbleManager::AddBubble(HEngine::POINT pt, int Power, bool Player)
{
	if (m_maxBubbleCount <= m_iBubbleCount && Player)
		return false;

	BUBBLESTRUCT* pNew = new BUBBLESTRUCT();
	pNew->fEtime = 0.0f;
	pNew->pt = pt;
	pNew->bitBubble = HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/BlueBub.bmp");
	pNew->Power = Power;
	pNew->bUpdate = false;
	if(Player)
		pNew->bPlayer = true;
	else 
		pNew->bPlayer = false;
	for (auto iter = m_vecBubble.begin(); iter != m_vecBubble.end(); iter++)
	{
		if ((*iter)->pt.x == pNew->pt.x && (*iter)->pt.y == pNew->pt.y && (*iter)->bitBubble != NULL)
		{
			SAFE_DELETE(pNew);
			return false;
		}
	}
	m_vecBubble.push_back(pNew);
	if(Player)
		m_iBubbleCount++;
	return true;
}

void BubbleManager::PlusBubbleCount()
{
	m_maxBubbleCount++;
}

int BubbleManager::GetBubbleCount()
{
	return m_maxBubbleCount;
}


bool BubbleManager::Update()
{
	for (auto iter = m_vecBubble.begin(); iter != m_vecBubble.end(); iter++)
	{
		(*iter)->fEtime += HEngine::TimeManager::GetInstance()->GetElipseTime();
		if ((*iter)->fEtime >= BUBBLE_BOOM_DURINGTIME + BUBBLE_BOOM_TIME)
		{
			if ((*iter)->bPlayer)
				m_iBubbleCount--;

			MapManager::GetInstance()->ClearBoom((*iter)->pt, (*iter)->Power);
			BUBBLESTRUCT* pDel = (*iter);
			SAFE_DELETE (pDel);
			m_vecBubble.erase(iter);
		
			break;
		}
		else if ((*iter)->fEtime >= BUBBLE_BOOM_TIME)
		{
			(*iter)->bUpdate = true;
			(*iter)->bitBubble = HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/Boomcenter.bmp");
			MapManager::GetInstance()->BoomBubble((*iter)->pt, (*iter)->Power);
		}
		
	}
	return false;
}

void BubbleManager::Draw()
{
	for (auto iter = m_vecBubble.begin(); iter != m_vecBubble.end(); iter++)
	{
		if((*iter)->bitBubble!= NULL)
			(*iter)->bitBubble->Draw((*iter)->pt.x * 40 + 20 , (*iter)->pt.y * 40 + 40);
	}
}

void BubbleManager::Release()
{
	for (auto iter = m_vecBubble.begin(); iter != m_vecBubble.end(); iter++)
	{
		SAFE_DELETE(*iter);
	}
	m_maxBubbleCount = 1;
}

BubbleManager::BubbleManager()
{
	HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/BlueBub.bmp")->SetAniMation(4, FALSE);
	HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/Boomcenter.bmp")->SetAniMation(3, TRUE);
	m_iBubbleCount = 0;
	m_maxBubbleCount = 1;
	temp = 0;
}


BubbleManager::~BubbleManager()
{
}
