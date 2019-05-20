#include "GameManager.h"
#include "ResManager.h"
#include "BitMap.h"
#include "BackGround.h"
#include "Mecro.h"
#include "Player.h"
#include "resource.h"
#include "Fire.h"

GameManager* GameManager::p_This = NULL;


GameManager::GameManager()
{
}


GameManager::~GameManager()
{
}

void GameManager::Init(HDC hdc, HINSTANCE g_hInst)
{
	BackDC = CreateCompatibleDC(hdc);
	m_hBitmap = CreateCompatibleBitmap(hdc, 1024, 450);
	m_Oldbitmap = (HBITMAP)SelectObject(BackDC, m_hBitmap);


	ResManager::GetInstance()->Init(hdc, g_hInst);
	m_BackGround = new BackGround();
	m_BackGround->Init(hdc);
	m_Player = new Player();
	m_Player->Init();
	m_Fire = new Fire();
	m_Fire->Init();
	
	m_bjump = false;
	m_Die = false;
	m_End = false;
	m_Count = 0;
}

void GameManager::Draw(HDC hdc)
{
	m_BackGround->Draw(BackDC);
	m_Fire->Draw(BackDC, m_Player);
	BitBlt(hdc, 0, 0, 1204, 450, BackDC, 0, 0, SRCCOPY);

}

void GameManager::Update(HWND hWnd)
{
	if (m_Die)
		return;
	m_BackGround->Save(m_Player->GetRect());

	if (m_Player->GetJump())
		m_Player->Jump();

	m_Fire->Update();

	if (m_Fire->Collider(m_Player->GetRect()))
	{
		if (m_Fire->GetWin())
		{
			m_End = true;
			m_Player->Win();
			m_BackGround->Win();
			return;
		}
		else if (m_Fire->GetCash())
		{
			m_BackGround->PlusScore(10000);
			m_Fire->SetBonus();
			return;
		}

		else
		{
			m_Die = true;
			m_Player->Die();
			m_BackGround->Die();
			m_Fire->Die();
			return;
		}
	}



}

void GameManager::Move(int key)
{
	if (m_End)
		return;

	if (!m_Die && key != VK_SPACE)
	{
		if (key == VK_RIGHT || key == VK_UP)
			m_BackGround->PlusScore(1);
		else if (key == VK_LEFT)
			m_BackGround->PlusScore(-1);

		m_Player->Move(key);
		m_BackGround->SetMove(m_Player->GetPosition());
		m_Fire->SetMove(m_Player->GetPosition());
	}

	else if(m_Die && key == VK_SPACE)
	{
		m_Player->SetPosition(m_BackGround->GetSavePoint());
		m_Fire->SetPosition(m_BackGround->GetSavePoint());
		m_BackGround->SetMove(m_Player->GetPosition());
		m_BackGround->SetLive();
		m_Die = false;
	}

}

void GameManager::Restart(HDC hdc)
{
	m_BackGround->Init(hdc);
	m_Player->Init();
	m_Fire->Init();
	m_bjump = false;
	m_Die = false;
	m_End = false;
	m_Count = 0;
}

void GameManager::Release()
{
	ResManager::GetInstance()->Release();
	SAFE_DELETE(m_BackGround);
	SAFE_DELETE(m_Player);
	SAFE_DELETE(p_This);

}