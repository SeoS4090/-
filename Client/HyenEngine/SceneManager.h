#pragma once
#include "GlobalDefine.h"
#include "SingleTone.h"
namespace HEngine
{
	class Scene;

	class SceneManager
	{
	private:
		bool				m_bInit;
		HWND				m_hWnd;
		vector<Scene*>		m_vecScene;
		Scene *				m_curScene;
		Scene *				m_lastScene;
		int					m_iWinCx;
		int					m_iWinCy;
		SceneManager();
		static SceneManager * s_This;
		bool				m_bPause;
	public:
		
		~SceneManager();
		static SceneManager*	GetInstance()
		{
			if (s_This == NULL)
			{
				s_This = new SceneManager();
			}

			return s_This;
		}
		void Init(HWND hWnd, int cx, int cy);
		void RegistScene(Scene * RegScene);
		bool Update();
		void Release();
		void LoadScene(int Scene_Num);
		void Pause();
		bool GetPause()
		{
			return m_bPause;
		}
		void SetStart()
		{
			m_bPause = false;
		}
	};
}