#include "AirPlanScene.h"
#include "ResourceManager.h"
#include "Defines.h"
#include "UIManager.h"
#include "InputManager.h"
#include "AirPlan.h"
#include "ObjectManager.h"
#include "FeverEffect.h"
AirPlanScene::AirPlanScene()
{
	m_player = new AirPlan();
	m_Fever = new FeverEffect();
}


AirPlanScene::~AirPlanScene()
{
	SAFE_DELETE(m_player);
	SAFE_DELETE(m_Fever);
}
 bool  Pausebutton()
{
	 if (HEngine::SceneManager::GetInstance()->GetPause())
	 {
		 HEngine::SceneManager::GetInstance()->SetStart();
	 }
	 else
		 HEngine::SceneManager::GetInstance()->Pause();
	return true;
}

void AirPlanScene::Init(HWND hWnd)
{
	m_strScore = "0";
	m_iStarScore = 0;

	m_Timeremaining = 1.0f;
	m_bPause = false;
	m_lasttime = 0;
	m_pBack = HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/airplanbackground.bmp");

	HEngine::UIManager::GetInstance()->ReleaseUI();
	
	bf_btpause = Pausebutton;

	HEngine::UIManager::GetInstance()->AddButton(451,0,"./RES/pausebutton.bmp",bf_btpause);

	HEngine::InputManager::GetInstance()->Clear();
	HEngine::InputManager::GetInstance()->Init(hWnd);
	HEngine::InputManager::GetInstance()->RegistKeyCode(VK_UP);
	HEngine::InputManager::GetInstance()->RegistKeyCode(VK_DOWN);
	HEngine::InputManager::GetInstance()->RegistKeyCode(VK_RIGHT);
	HEngine::InputManager::GetInstance()->RegistKeyCode(VK_LEFT);
	HEngine::InputManager::GetInstance()->RegistKeyCode(VK_LBUTTON);

	pt_plan.x = 200;
	pt_plan.y = 600;
	m_player->Init(pt_plan);


	m_duringtime = 0;
	m_strTime = GAME_MAXTIME;
	m_state = STATE_READY;
	
}

bool AirPlanScene::Input(float fEtime)
{
	if (m_state != STATE_GAME)
		return false;

	if (!m_player->GetDie())
	{
		if (HEngine::InputManager::GetInstance()->isKeyPress(VK_UP))
			pt_plan.y -= fEtime * AIRPLAN_SPEED;
		if (HEngine::InputManager::GetInstance()->isKeyPress(VK_DOWN))
			pt_plan.y += fEtime * AIRPLAN_SPEED;
		if (HEngine::InputManager::GetInstance()->isKeyPress(VK_LEFT))
			pt_plan.x -= fEtime * AIRPLAN_SPEED;
		if (HEngine::InputManager::GetInstance()->isKeyPress(VK_RIGHT))
			pt_plan.x += fEtime * AIRPLAN_SPEED;
	}

	if (pt_plan.x > GAME_WIDTH - HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/airplan.bmp")->GetWidth())
		pt_plan.x = GAME_WIDTH - HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/airplan.bmp")->GetWidth();
	if (pt_plan.x < 0)
		pt_plan.x = 0;

	if (pt_plan.y > GAME_HEIGHT_END - HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/airplan.bmp")->GetHeight())
		pt_plan.y = GAME_HEIGHT_END - HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/airplan.bmp")->GetHeight();
	if (pt_plan.y < GAME_HEIGHT_START)
		pt_plan.y = GAME_HEIGHT_START;


	return false;
}

void AirPlanScene::Draw(HDC hdc)
{
	m_pBack->DrawBitblt(0, 0);
	m_player->Draw();
	ObjectManager::GetInstance()->Draw();
	HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/airplanui.bmp")->Draw(0, 0);
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

void AirPlanScene::Update(float fEtime)
{
	HEngine::UIManager::GetInstance()->ReleaseUI();

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
		HEngine::UIManager::GetInstance()->AddButton(110, 705, "./RES/Confirm.bmp", bf_btpause);
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
			m_player->Die();
			return;
		}
		// 남은시간 계산
		time -= fEtime;
		m_Timeremaining = 1.0f - (stof(GAME_MAXTIME) - time) / stof(GAME_MAXTIME);
		//object 업데이트
		ColliderType bCollider = ObjectManager::GetInstance()->Update(fEtime, m_player->GetRect());
		m_Fever->Update(fEtime, bCollider);
		//총알 생성
		if(int(m_duringtime*100) % int(BULLET_RESPAWN_TIME *100) == 0 && !m_player->GetDie())
			ObjectManager::GetInstance()->AddBullet();
		//점수 생성
		if ((int(m_duringtime * 100) % int(SCORE_RESPAWN_TIME * 10) == 0 && !m_player->GetDie()) || m_Fever->GetFever())
			ObjectManager::GetInstance()->AddStar(m_Fever->GetFever());
		
		if (bCollider.m_bCollider)
		{
			if (bCollider.type == OBJECT_BULLET)
			{
				m_player->Die();
				m_duringtime = 0;
				m_iStarScore = 0;
			}
			else if (bCollider.type == OBJECT_STAR)
			{
				m_iStarScore += 100;
				int m_iscore = stoi(m_strScore); 
				m_iscore += m_iStarScore;
				char Buf[101];
				std::sprintf(Buf, "%d", m_iscore);
				m_strScore = Buf;
			}
		}

		char Buf[101];
		std::sprintf(Buf, "%.2f", time);
		m_strTime = Buf;
		HEngine::UIManager::GetInstance()->AddLabel(m_strTime, (GAME_WIDTH - (33 * m_strTime.size())) / 2, 800 - 45, DT_CENTER | DT_WORDBREAK, true);
		HEngine::UIManager::GetInstance()->AddButton(451, 0, "./RES/pausebutton.bmp", bf_btpause);
		HEngine::UIManager::GetInstance()->AddLabel(m_strScore, (GAME_WIDTH - (33 * m_strScore.size())) / 2, 0, DT_CENTER | DT_WORDBREAK, true);

	}

	if (m_player->GetDie() && m_duringtime > 1.5f)
	{
		m_player->Resurrection();
		pt_plan.x = 200;
		pt_plan.y = 600;
	}

	m_player->Update(fEtime, pt_plan);


}


void AirPlanScene::Release()
{
	SAFE_DELETE(m_player);
	SAFE_DELETE(m_Fever);
	ObjectManager::GetInstance()->ReleaseObject();
	ObjectManager::GetInstance()->DestroyInstance();
	HEngine::InputManager::GetInstance()->Clear();
	HEngine::InputManager::GetInstance()->DestroyInstance();
}

