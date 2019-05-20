#include<Windows.h>
#include<crtdbg.h>
#include "GameManager.h"
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("서커스 게임 1단계");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPervlnstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;
	//WndClass는 기본 윈도우환경을 만드는 구조체다. 맴버변수는 밑에와 같다.
	WndClass.cbClsExtra = 0; //예약영역
	WndClass.cbWndExtra = 0; //예약영역 (신경x)
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	//배경색
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);	//커서
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);	//아이콘 모양
	WndClass.hInstance = hInstance; //(프로그램 핸들값(번호)등록)
	WndClass.lpfnWndProc = WndProc;	//프로세스 함수 호출
	WndClass.lpszClassName = lpszClass;	//클레스 이름
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);  //만들어진 WidClass를 등록

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, NULL, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{

	HDC hdc;
	PAINTSTRUCT ps;
	switch (iMessage)
	{
	case WM_CREATE:
		SetTimer(hWnd, 1, 10, NULL);
		hdc = GetDC(hWnd);
		GameManager::GetInstance()->Init(hdc , g_hInst);
		ReleaseDC(hWnd, hdc);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		GameManager::GetInstance()->Draw(hdc);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_TIMER:

		hdc = GetDC(hWnd);
		GameManager::GetInstance()->Update(hWnd);


		if (GetKeyState(VK_RIGHT) & 0x8000)
			GameManager::GetInstance()->Move(VK_RIGHT);
		if (GetKeyState(VK_LEFT) & 0x8000)
			GameManager::GetInstance()->Move(VK_LEFT);
		if (GetKeyState(VK_UP) & 0x8000)
			GameManager::GetInstance()->Move(VK_UP);
		if (GetKeyState(VK_SPACE) & 0x8000)
		{
			GameManager::GetInstance()->Move(VK_SPACE);
			if (GameManager::GetInstance()->GetEnd())
				GameManager::GetInstance()->Restart(hdc);
		}
	
		
		
		GameManager::GetInstance()->Move(0);

		ReleaseDC(hWnd, hdc);
		InvalidateRect(hWnd, NULL, FALSE);
		return 0;
	case WM_DESTROY:
		GameManager::GetInstance()->Release();
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}