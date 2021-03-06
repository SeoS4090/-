#include<Windows.h>
#include "resource.h"
#include"GameManager.h"
#include "ResManager.h"
#include<crtdbg.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("지뢰 찾기");
INT_PTR CALLBACK	Mode(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPervlnstance, LPSTR lpszCmdParam, int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtDumpMemoryLeaks();
	//_CrtSetBreakAlloc(156); //중단점 찾기

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
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	WndClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	RegisterClass(&WndClass);  //만들어진 WidClass를 등록

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 885, 570,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, NULL, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}
int mode;


LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	switch (iMessage)
	{
	case WM_CREATE:
		SetTimer(hWnd, 1, 1000, NULL);
		hdc = GetDC(hWnd);
		ResManager::GetInstance()->Init(hdc, g_hInst);
		GameManager::GetInstance()->Init(hdc, g_hInst);
		ReleaseDC(hWnd, hdc);
		return 0;
	case WM_TIMER:
		GameManager::GetInstance()->Timer();
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		GameManager::GetInstance()->Draw(hdc);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_LBUTTONDBLCLK:
		GameManager::GetInstance()->LBDClick(hWnd, LOWORD(lParam), HIWORD(lParam));
		return 0;
	case WM_LBUTTONDOWN:

		GameManager::GetInstance()->LBClick(hWnd, LOWORD(lParam), HIWORD(lParam));
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;

	case WM_RBUTTONDOWN:
		GameManager::GetInstance()->RBClick(hWnd, LOWORD(lParam), HIWORD(lParam));
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;

	case WM_COMMAND:
		hdc = GetDC(hWnd);
		switch (LOWORD(wParam))
		{
		case ID_MODE:
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, Mode);
			break;
		}
		GameManager::GetInstance()->SetMode(hdc, g_hInst, mode);
		ReleaseDC(hWnd, hdc);
		return 0;
	case WM_DESTROY:
		GameManager::GetInstance()->Release();
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
static int GameMode = IDC_EASY;
INT_PTR CALLBACK Mode(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		CheckRadioButton(hDlg, IDC_EASY, IDC_HARD, GameMode);
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:	
			if (IsDlgButtonChecked(hDlg, IDC_EASY))
			{
				mode = 1;
				GameMode = IDC_EASY;
				MessageBox(hDlg, "이지", "모드", MB_OK);
			}
			if (IsDlgButtonChecked(hDlg, IDC_NOMAL))
			{
				mode = 2;
				GameMode = IDC_NOMAL;
				MessageBox(hDlg, "노말", "모드", MB_OK);
			}
			if (IsDlgButtonChecked(hDlg, IDC_HARD))
			{
				mode = 3;
				GameMode = IDC_HARD;
				MessageBox(hDlg, "하드", "모드", MB_OK);
			}

			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		case IDCANCEL:
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;

		}
		break;
	}
	return (INT_PTR)FALSE;
}
