#pragma once
#include "SingleTone.h"
#include "GlobalDefine.h"
namespace HEngine
{
	class TimeManager : public SingleTone<TimeManager>
	{
	private:
		bool				m_bUseFPS;
		int					m_nFPS;
		DWORD				m_dwLastTime;
		DWORD				m_dwCurTime;
		float				m_fElapseTime;
	public:
		void				init(bool useFPS, int FPS);
		bool				Update();
		inline float		GetElipseTime()
		{
			return m_fElapseTime;
		}
		TimeManager();
		~TimeManager();
	};
}
