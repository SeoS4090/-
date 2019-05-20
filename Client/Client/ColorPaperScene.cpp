#include "ColorPaperScene.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "Defines.h"
#include "UIManager.h"
#include "EngineMain.h"
#include "ObjectManager.h"
#include "Paper.h"
#include "FeverEffect.h"

bool Pausebuttonclick()
{
	if (HEngine::SceneManager::GetInstance()->GetPause())
	{
		HEngine::SceneManager::GetInstance()->SetStart();
	}
	else
		HEngine::SceneManager::GetInstance()->Pause();
	return true;
}
void ColorPaperScene::Init(HWND hWnd)
{
	m_strScore = "0";
	m_iScore = 0;
	m_strTime = GAME_MAXTIME;
	m_Timeremaining = 1.0f;
	m_bPause = false;
	m_lasttime = 0;
	m_state = STATE_READY;
	m_duringtime = 0;

	HEngine::UIManager::GetInstance()->ReleaseUI();

	HEngine::InputManager::GetInstance()->Clear();
	HEngine::InputManager::GetInstance()->RegistKeyCode(VK_UP);
	HEngine::InputManager::GetInstance()->RegistKeyCode(VK_DOWN);
	HEngine::InputManager::GetInstance()->RegistKeyCode(VK_LEFT);
	HEngine::InputManager::GetInstance()->RegistKeyCode(VK_RIGHT);
	HEngine::InputManager::GetInstance()->RegistKeyCode(VK_LBUTTON);

	m_pBack = HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/PaperGame/colorpaper.bmp");
	pause = Pausebuttonclick;
	HEngine::UIManager::GetInstance()->AddButton(451, 0, "./RES/pausebutton.bmp", pause);
	
	ObjectManager::GetInstance()->AddPaper(false);
}

bool ColorPaperScene::Input(float fEtime)
{
	if (m_state != STATE_READY && m_state != STATE_GO)
	{
		if (HEngine::InputManager::GetInstance()->isKeyDown(VK_UP))
		{
			if (!ObjectManager::GetInstance()->Input(VK_UP))
				m_Fever->MinusFeverRate(0.2f);
			
		}
		else if (HEngine::InputManager::GetInstance()->isKeyDown(VK_DOWN))
		{
			if (!ObjectManager::GetInstance()->Input(VK_DOWN))
				m_Fever->MinusFeverRate(0.2f);
		}
		else if (HEngine::InputManager::GetInstance()->isKeyDown(VK_LEFT))
		{
			if (!ObjectManager::GetInstance()->Input(VK_LEFT))
				m_Fever->MinusFeverRate(0.2f);
		}
		else if (HEngine::InputManager::GetInstance()->isKeyDown(VK_RIGHT))
		{
			if (!ObjectManager::GetInstance()->Input(VK_RIGHT))
				m_Fever->MinusFeverRate(0.2f);
		}
	}
	return false;
}

void ColorPaperScene::Draw(HDC hdc)
{
	m_pBack->DrawBitblt(0, 0);
	ObjectManager::GetInstance()->Draw();
	HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/timebar.bmp")->Draw(25, 800 - 40, m_Timeremaining);

	HEngine::UIManager::GetInstance()->Draw();
	HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/clock.bmp")->Draw(120, 800 - 45);
	m_Fever->Draw();
	if (m_state == STATE_PAUSE)
		HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/pause.bmp")->DrawBitblt(0, 0);
	else if (m_state == STATE_END)
		HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/timeover.bmp")->Draw((GAME_WIDTH - HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/timeover.bmp")->GetWidth()) / 2, (GAME_HEIGHT_END - GAME_HEIGHT_START - HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/timeover.bmp")->GetHeight()) / 2 + GAME_HEIGHT_START);
	else if (m_state == STATE_READY)
		HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/ready.bmp")->Draw((GAME_WIDTH - HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/ready.bmp")->GetWidth()) / 2, (GAME_HEIGHT_END - GAME_HEIGHT_START - HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/ready.bmp")->GetHeight()) / 2 + GAME_HEIGHT_START);
	else if (m_state == STATE_GO)
		HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/go.bmp")->Draw((GAME_WIDTH - HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/go.bmp")->GetWidth()) / 2, (GAME_HEIGHT_END - GAME_HEIGHT_START - HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/go.bmp")->GetHeight()) / 2 + GAME_HEIGHT_START);

}

void ColorPaperScene::Update(float fEtime)
{
	HEngine::UIManager::GetInstance()->ReleaseUI();

	//시간 계산
	float time = stof(m_strTime.c_str());
	m_duringtime += fEtime;

	//게임 종료
	if (m_state == STATE_END)
	{
		m_duringtime += fEtime;
		if (m_duringtime > 3.0f)
			HEngine::SceneManager::GetInstance()->LoadScene(SCENE_SELECTGAME);

		return;
	}
	//일시정지
	if (HEngine::SceneManager::GetInstance()->GetPause())
	{
		m_state = STATE_PAUSE;
		HEngine::UIManager::GetInstance()->ReleaseUI();
		HEngine::UIManager::GetInstance()->AddButton(110, 705, "./RES/Confirm.bmp", pause);
		return;
	}
	//레디 고 상황
	else if (m_state == STATE_READY || m_state == STATE_GO)
	{
		m_duringtime += fEtime;
		if (m_duringtime >= 2.5f)
		{
			m_state = STATE_GAME;
			m_duringtime = 0;

		}
		else if (m_duringtime >= 1.5f)
			m_state = STATE_GO;
	}
	//게임 시작
	else if (m_state != STATE_READY && m_state != STATE_GO)
	{
		m_state = STATE_GAME;
		float time = stof(m_strTime.c_str());
		m_duringtime += fEtime;

		//게임 끝날때
		if (time <= 0.0f)
		{
			m_state = STATE_END;
			m_duringtime = 0;
			return;
		}
		// 남은시간 계산
		time -= fEtime;
		m_Timeremaining = 1.0f - (stof(GAME_MAXTIME) - time) / stof(GAME_MAXTIME);


		char Buf[101];
		std::sprintf(Buf, "%.2f", time);
		m_strTime = Buf;
		HEngine::UIManager::GetInstance()->AddLabel(m_strTime, (GAME_WIDTH - (33 * m_strTime.size())) / 2, 800 - 45, DT_CENTER | DT_WORDBREAK, true);
		HEngine::UIManager::GetInstance()->AddButton(451, 0, "./RES/pausebutton.bmp", pause);
		HEngine::UIManager::GetInstance()->AddLabel(m_strScore, (GAME_WIDTH - (33 * m_strScore.size())) / 2, 0, DT_CENTER | DT_WORDBREAK, true);


		Paper_Position direct;
		ColliderType collidertype = ObjectManager::GetInstance()->Update(fEtime, { 0,0,0,0 });
		m_Fever->Update(fEtime, collidertype);

		if (collidertype.type == OBJECT_PAPER && collidertype.m_bCollider)
		{
			ObjectManager::GetInstance()->AddPaper(m_Fever->GetFever());
			m_iScore += ObjectManager::GetInstance()->GetScore();
			//점수 업
			m_iScore += 100;

			char buf[64];
			sprintf(buf, "%d", m_iScore);
			m_strScore = buf;
		}
				
	}

	
}

void ColorPaperScene::Release()
{
	ObjectManager::GetInstance()->ReleaseObject();
}


ColorPaperScene::ColorPaperScene()
{
	m_Fever = new FeverEffect();
}


ColorPaperScene::~ColorPaperScene()
{
	SAFE_DELETE(m_Fever);
}
