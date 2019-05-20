#include "SelectGameScene.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "Defines.h"
#include "UIManager.h"
SelectGameScene::SelectGameScene()
{
}


SelectGameScene::~SelectGameScene()
{
}

bool BackButton()
{
	HEngine::SceneManager::GetInstance()->LoadScene(SCENE_TITLE);
	return true;
}
bool ColorPaper()
{
	HEngine::SceneManager::GetInstance()->LoadScene(SCENE_COLORPAPER);
	return true;
}
bool AirPlane()
{
	HEngine::SceneManager::GetInstance()->LoadScene(SCENE_AIRPLAN);
	return true;
}

void SelectGameScene::Init(HWND hWnd)
{
	HEngine::UIManager::GetInstance()->ReleaseUI();
	HEngine::InputManager::GetInstance()->Clear();
	HEngine::InputManager::GetInstance()->Init(hWnd);
	HEngine::InputManager::GetInstance()->RegistKeyCode(VK_BACK);
	HEngine::InputManager::GetInstance()->RegistKeyCode(VK_LBUTTON);
	
	m_pBack = HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/selectgame.bmp");
	bf_btBack = BackButton;
	bf_btAirePlane = AirPlane;
	bf_btColorpapaer = ColorPaper;
	HEngine::UIManager::GetInstance()->AddButton(10, 740, "./RES/backbutton.bmp", bf_btBack);
	HEngine::UIManager::GetInstance()->AddButton(395, 309, "./RES/dogamebutton.bmp", bf_btColorpapaer);
	HEngine::UIManager::GetInstance()->AddButton(395, 411, "./RES/dogamebutton.bmp", bf_btAirePlane);

}

bool SelectGameScene::Input(float fETime)
{
	if (HEngine::InputManager::GetInstance()->isKeyUp(VK_BACK))
		HEngine::SceneManager::GetInstance()->LoadScene(SCENE_TITLE);

	return false;
}

void SelectGameScene::Update(float fETime)
{
}

void SelectGameScene::Draw(HDC hdc)
{
	m_pBack->DrawBitblt(0, 0);
	HEngine::UIManager::GetInstance()->Draw();
}

void SelectGameScene::Release()
{
	HEngine::UIManager::GetInstance()->ReleaseUI();
	HEngine::InputManager::GetInstance()->Clear();
}

