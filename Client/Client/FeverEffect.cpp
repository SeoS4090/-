#include "FeverEffect.h"
#include "ResourceManager.h"


void FeverEffect::Draw()
{
	if (m_bFever)
	{
		HEngine::ResourceManager::GetInstance()->GetBitmap(m_sFeverRes[(m_iFeverRes + 1) % 2])->Draw(25, 68);
		for (int i = 0; i < 9; i++)
				for (int j = 0; j < 10; j++)
				{
					if (j < 9 && j > 0 && i > 0 && i < 8)
						continue;
					if (int(m_FeverTime * 10) % 2 == 0)
						HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/feverefect02.bmp")->Draw((GAME_WIDTH / 9) * i, GAME_HEIGHT_START + ((GAME_HEIGHT_END - GAME_HEIGHT_START) / 10) * j);
					else
						HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/feverefect.bmp")->Draw((GAME_WIDTH / 9) * i, GAME_HEIGHT_START + ((GAME_HEIGHT_END - GAME_HEIGHT_START) / 10) * j);
			}
		
	}
	HEngine::ResourceManager::GetInstance()->GetBitmap(m_sFeverRes[m_iFeverRes])->Draw(25, 68, m_fFeverRateDraw);
}

bool FeverEffect::Update(float fEtime, ColliderType collider)
{
	if (m_bFever)
	{
		m_FeverTime += fEtime;
		if (m_FeverTime >= 5.0f)
		{
			m_bFever = false;
			m_FeverTime = 0;
			m_fFeverPercent = 0.0f;
			m_iFeverRes = 0;
		}
	}
	if (m_fFeverPercent > m_fFeverRateDraw)
	{
		m_fFeverRateDraw += fEtime;
		if (m_fFeverPercent < m_fFeverRateDraw)
			m_fFeverRateDraw = m_fFeverPercent;
	}

	else if (m_fFeverPercent < m_fFeverRateDraw)
	{
		m_fFeverRateDraw -= fEtime;

		if (m_fFeverPercent > m_fFeverRateDraw)
			m_fFeverRateDraw = m_fFeverPercent;
	}
	
	m_fFeverPercent += fEtime / 100;

	if (collider.m_bCollider == true && collider.type == OBJECT_STAR)
		m_fFeverPercent += 0.2f;

	if (collider.m_bCollider == true && collider.type == OBJECT_BULLET)
	{
		m_fFeverPercent = 0;
		m_FeverTime = 0.0f;
		m_iFeverRes = 0;
		m_bFever = false;
	}
	
	
	if (collider.m_bCollider == true && collider.type == OBJECT_PAPER)
		m_fFeverPercent += 0.15f;

	if (m_fFeverPercent >= 1.0f)
	{
		m_fFeverRateDraw = 0;
		m_fFeverPercent = 0.0f;
		m_bFever = true;
		m_iFeverRes++;
		m_iFeverRes = m_iFeverRes % 2;
	}

	return true;
}

void FeverEffect::MinusFeverRate(float rate)
{

	m_fFeverPercent -= rate;
	if (m_fFeverPercent < 0.0000f)
		m_fFeverPercent = 0.0f;

	if (m_bFever)
	{
		m_bFever = false;
		m_fFeverPercent = 0.0f;
		m_iFeverRes = 0;
	}
	
}

FeverEffect::FeverEffect()
{
	m_FeverTime = 0.0f;
	m_pt.x = 30;
	m_pt.y = 68;
	m_fFeverPercent = 0;
	m_sFeverRes[0] = "./RES/fever00.bmp";
	m_sFeverRes[1] = "./RES/fever01.bmp";
	m_fFeverRateDraw = 0.0f;
	m_iFeverRes = 0;
	m_bFever = false;
}


FeverEffect::~FeverEffect()
{
}
