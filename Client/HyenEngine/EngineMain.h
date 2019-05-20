#pragma once
#include "GlobalDefine.h"
#include "SceneManager.h"


namespace HEngine
{
	class EngineMain
	{
	private:
		string		m_strTitle;
		int			m_winSizeX;
		int			m_winSizeY;
	public:
		int			StartEngine(HINSTANCE hInstance);
		EngineMain(string title, int cx, int cy);
		~EngineMain();
	};
}
