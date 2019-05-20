#include "TitleScene.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "BitMap.h"
#include "Defines.h"

void TitleScene::Init(HWND hWnd)
{
	HEngine::InputManager::GetInstance()->Clear();
	HEngine::InputManager::GetInstance()->RegistKeyCode(VK_SPACE);
	
	m_pBack = HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/start.bmp");
	m_hWnd = hWnd;
}

bool TitleScene::Input(float fETime)
{
	if (HEngine::InputManager::GetInstance()->isKeyUp(VK_SPACE))
		HEngine::SceneManager::GetInstance()->LoadScene(SCENE_SELECTGAME);
	else if (HEngine::InputManager::GetInstance()->isKeyUp(VK_ESCAPE))
		return true;
	return false;
}

void TitleScene::Update(float fETime)
{
}

void TitleScene::Draw(HDC hdc)
{
	m_pBack->DrawBitblt(0, 0);
}

void TitleScene::Release()
{
	HEngine::InputManager::GetInstance()->Clear();
	HEngine::InputManager::GetInstance()->DestroyInstance();
}



TitleScene::TitleScene()
{
}


TitleScene::~TitleScene()
{
}
