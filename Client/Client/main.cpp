#include <Windows.h>
#include "EngineMain.h"
#include "TitleScene.h"
#include "SelectGameScene.h"
#include "ColorPaperScene.h"
#include "AirPlanScene.h"
#include <crtdbg.h>

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	_CrtDumpMemoryLeaks();
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(304);
	//���� ��ü�� �����ϰ� Ÿ��Ʋ�� ������ ����� �־��ش�.
	HEngine::EngineMain engine("Every Game", 514, 800);
	//���� ����Ѵ�. ù��° ����ϴ� ���� �ʱ�ȭ���� �ȴ�.
	HEngine::SceneManager::GetInstance()->RegistScene(new TitleScene);
	HEngine::SceneManager::GetInstance()->RegistScene(new SelectGameScene);
	HEngine::SceneManager::GetInstance()->RegistScene(new ColorPaperScene);
	HEngine::SceneManager::GetInstance()->RegistScene(new AirPlanScene);

	
	//���� ����
	return engine.StartEngine(hInstance);
}
