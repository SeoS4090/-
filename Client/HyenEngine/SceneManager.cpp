#include "SceneManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "UIManager.h"

namespace HEngine
{
	SceneManager* SceneManager::s_This = NULL;


	SceneManager::SceneManager()
	{
		m_bInit = false;
	}


	SceneManager::~SceneManager()
	{
	}
	void SceneManager::Init(HWND hWnd, int cx, int cy)
	{
		m_hWnd = hWnd;
		::RECT rcClient, rcWindow;
		m_iWinCx = cx;
		m_iWinCy = cy;

		GetClientRect(hWnd, &rcClient);
		GetWindowRect(hWnd, &rcWindow);

		int m_iFrameCx = (rcWindow.right - rcWindow.left) - (rcClient.right - rcClient.left);
		int m_iFrameCy = (rcWindow.bottom - rcWindow.top) - (rcClient.bottom - rcClient.top);

		MoveWindow(hWnd, 0, 0, m_iWinCx + m_iFrameCx, m_iWinCy + m_iFrameCy, true);
		HDC hdc = GetDC(m_hWnd);

		HEngine::ResourceManager::GetInstance()->initBack(hdc, m_iWinCx, m_iWinCy);
		HEngine::InputManager::GetInstance()->Init(hWnd);

		ReleaseDC(hWnd, hdc);
		m_curScene = m_vecScene[0];
		m_curScene->Init(hWnd);
		
		m_bPause = false;
	}
	void SceneManager::RegistScene(Scene * RegScene)
	{
		
		m_vecScene.push_back(RegScene);
	}
	bool SceneManager::Update()
	{
		if (!HEngine::TimeManager::GetInstance()->Update())
			return false;

		HEngine::InputManager::GetInstance()->Update();
		HEngine::UIManager::GetInstance()->Update();


		if (m_curScene->Input(HEngine::TimeManager::GetInstance()->GetElipseTime()))
			return true;

		m_curScene->Update(HEngine::TimeManager::GetInstance()->GetElipseTime());

		HDC hdc = GetDC(m_hWnd);
		HEngine::ResourceManager::GetInstance()->DrawBack(hdc);
		m_curScene->Draw(hdc);
		ReleaseDC(m_hWnd, hdc);
		return false;
	}
	void SceneManager::Release()
	{
		for (auto iter = m_vecScene.begin(); iter != m_vecScene.end();)
		{
			Scene* pDelete = *iter;
			pDelete->Release();
			iter++;
			SAFE_DELETE(pDelete);
		}
		HEngine::TimeManager::GetInstance()->DestroyInstance();
		HEngine::UIManager::GetInstance()->DestroyInstance();
		HEngine::InputManager::GetInstance()->DestroyInstance();
		HEngine::ResourceManager::GetInstance()->DestroyInstance();
		SAFE_DELETE(s_This);
	}
	void SceneManager::LoadScene(int SceneIndex)
	{
		m_lastScene = m_curScene;
		m_curScene = m_vecScene[SceneIndex];

		m_lastScene->Release();
		m_curScene->Init(m_hWnd);
	}
	void SceneManager::Pause()
	{
		m_bPause = true;
	}
}