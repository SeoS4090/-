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
	//엔진 객체를 생성하고 타이틀과 윈도우 사이즈를 넣어준다.
	HEngine::EngineMain engine("Every Game", 514, 800);
	//씬을 등록한다. 첫번째 등록하는 씬이 초기화면이 된다.
	HEngine::SceneManager::GetInstance()->RegistScene(new TitleScene);
	HEngine::SceneManager::GetInstance()->RegistScene(new SelectGameScene);
	HEngine::SceneManager::GetInstance()->RegistScene(new ColorPaperScene);
	HEngine::SceneManager::GetInstance()->RegistScene(new AirPlanScene);

	
	//엔진 시작
	return engine.StartEngine(hInstance);
}
