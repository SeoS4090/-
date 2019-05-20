#include "TimeManager.h"


namespace HEngine
{
	void TimeManager::init(bool useFPS, int FPS)
	{
		m_bUseFPS = useFPS;
		m_nFPS = FPS;
	}
	bool TimeManager::Update()
	{
		m_dwCurTime = GetTickCount();
		if (m_bUseFPS)
		{
			if (m_dwCurTime - m_dwLastTime < 1000 / m_nFPS)
				return false;
		}
		m_fElapseTime = (m_dwCurTime - m_dwLastTime) / 1000.0f;
		m_dwLastTime = m_dwCurTime;

		return true;
	}
	TimeManager::TimeManager()
	{
		m_bUseFPS = false;
		m_nFPS = 60;

		m_dwCurTime = GetTickCount();
		m_dwLastTime = GetTickCount();

	}


	TimeManager::~TimeManager()
	{
	}
}