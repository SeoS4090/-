#include "AirPlan.h"
#include "ResourceManager.h"
#include "AirPlan.h"


AirPlan::AirPlan()
{
}
void AirPlan::Init(HEngine::POINTF pt)
{
	m_Player[0] = HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/airplan.bmp");
	m_Player[1] = HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/airplan2.bmp");

	m_Explosion[0] = HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/explosion01.bmp");
	m_Explosion[1] = HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/explosion02.bmp");
	m_Explosion[2] = HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/explosion03.bmp");
	m_bDie =false;
	m_iDieCount = 0;
	m_DuringTime = 0;
	m_point = pt;

}

void AirPlan::Update(float fEtime, HEngine::POINTF pt)
{
	m_DuringTime += fEtime;
	m_point = pt;

	if (m_bDie && m_DuringTime > 0.5f && m_DuringTime < 1.0f)
		m_iDieCount = 1;
	else if (m_bDie && m_DuringTime > 1.0f && m_DuringTime < 1.5f)
		m_iDieCount = 2;
}

void AirPlan::Draw()
{
	if (!m_bDie)
		m_Player[(int)(m_DuringTime*100)%2]->Draw(m_point);
	else
		m_Explosion[m_iDieCount]->Draw(m_point);
}

void AirPlan::Die()
{
	if (m_bDie && m_iDieCount<2)
		m_iDieCount++;
	else
	{
		m_bDie = true;
		m_iDieCount = 0;
		m_DuringTime = 0;
	}
}

void AirPlan::Resurrection()
{
	m_bDie = false;

}

HEngine::RECT AirPlan::GetRect()
{
	HEngine::RECT rt;
	rt.left = m_point.x + (m_Player[0]->GetWidth()/3);
	rt.top = m_point.y + (m_Player[0]->GetHeight() / 4);
	rt.right = rt.left + (m_Player[0]->GetWidth()/3);
	rt.bottom = rt.top + m_Player[0]->GetHeight()/2;

	return rt;
}

AirPlan::~AirPlan()
{
}
