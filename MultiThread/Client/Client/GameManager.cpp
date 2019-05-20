#include "GameManager.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "UIManager.h"
#include "TimeManager.h"
#include "MapManager.h"
#include "BubbleManager.h"

bool SetLoginRT(int arg);
bool SetGameStart(int arg);
bool IntoWaitRoom(int RoomNo);

bool SetLoginRT(int arg)
{
	PACKET_LOGIN packet;
	packet.header.wIndex = PACKET_INDEX_LOGIN;
	string id = GameManager::GetInstance()->GetID();
	if (id == "")
		return false;

	memcpy(packet.ID, id.c_str(), id.size());

	packet.validID = id.size();
	packet.ID[packet.validID] = 0;
	packet.header.wLen = sizeof(PACKET_HEADER) + sizeof(packet.ID) + sizeof(int);
	send(GameManager::GetInstance()->GetServerSock(), (char *)&packet, packet.header.wLen, 0);
	GameManager::GetInstance()->SetRoomNo(-1);
	return true;
}

bool SetGameStart(int arg)
{
	HEngine::UIManager::GetInstance()->ReleaseButton("./RES/GameReadyButton.bmp");
	PACKET_GAMEREADY packet;
	packet.header.wIndex = PACKET_INDEX_GAMEREADY;
	packet.header.wLen = sizeof(PACKET_HEADER) + sizeof(int);
	packet.RoomNo = GameManager::GetInstance()->GetRoomNo();
	
	send(GameManager::GetInstance()->GetServerSock(), (char *)&packet, packet.header.wLen, 0);

	MapManager::GetInstance()->Release();
	

	BubbleManager::GetInstance()->Release();
	
	HEngine::UIManager::GetInstance()->AddButton(800 - 145 - 50, 30, "./RES/BackButton.bmp", SetLoginRT, 0, false);

	return true;
}

bool IntoWaitRoom(int RoomNo)
{
	PACKET_WAITROOM packet;
	packet.header.wIndex = PACKET_INDEX_WAITROOM;
	packet.header.wLen = sizeof(PACKET_HEADER) + sizeof(int);
	packet.RoomNo = RoomNo;
	GameManager::GetInstance()->SetRoomNo(RoomNo);
	send(GameManager::GetInstance()->GetServerSock(), (char *)&packet, packet.header.wLen, 0);

	return true;
}


void GameManager::Init(HWND hWnd, int cx, int cy)
{
	m_hWnd = hWnd;
	//창 크기 조절
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
	HEngine::InputManager::GetInstance()->RegistKeyCode(VK_LBUTTON);
	ReleaseDC(hWnd, hdc);

	//데이터 초기화
	m_iState = LOGIN;
	HEngine::ResourceManager::GetInstance()->SetAnimation("./RES/Login.bmp", 2, FALSE);
	HEngine::POINT pt = { HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/Login.bmp")->GetWidth() / 2, HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/Login.bmp")->GetHeight() };
	HEngine::UIManager::GetInstance()->AddButton((m_iWinCx - pt.x) / 2, (m_iWinCy - pt.y) / 2, "./RES/Login.bmp", SetLoginRT, 0, true);
	HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/Cursur2.bmp")->SetAniMation(6, false);
	for (int i = 0; i < BUFSIZE; i++)
	{
		m_StrBuf[i] = 0;
	}

	g_iLen = 0;
	m_iMessageLine = 0;
	m_sID = "";
	m_iRoomNo = -1;
	m_iWaitingRoomPlayer = 0;
	m_Player = new character();
	m_iBubbleCount = 1;
	m_bInputKor = false;
}

void GameManager::Draw()
{

	HBRUSH Brush = CreateSolidBrush(RGB(16, 233, 255));
	HBRUSH oBrush = (HBRUSH)SelectObject(HEngine::ResourceManager::GetInstance()->GetBackDC(), Brush);
	Rectangle(HEngine::ResourceManager::GetInstance()->GetBackDC(), 0, 0, m_iWinCx, m_iWinCy);
	SelectObject(HEngine::ResourceManager::GetInstance()->GetBackDC(), oBrush);
	DeleteObject(Brush);
	switch (m_iState)
	{
	case LOGIN:
		TextOut(HEngine::ResourceManager::GetInstance()->GetBackDC(), 300, 450, "ID : _________________", strlen("ID : _________________"));
		TextOut(HEngine::ResourceManager::GetInstance()->GetBackDC(), 330, 450, m_StrBuf, strlen(m_StrBuf));
		break;

	case LOGINRT:
	{
		HBRUSH Brush = CreateSolidBrush(RGB(16, 255, 255));
		HBRUSH oBrush = (HBRUSH)SelectObject(HEngine::ResourceManager::GetInstance()->GetBackDC(), Brush);
		Rectangle(HEngine::ResourceManager::GetInstance()->GetBackDC(), 50, 60, m_iWinCx - 50, m_iWinCy - 60);
		SelectObject(HEngine::ResourceManager::GetInstance()->GetBackDC(), oBrush);
		DeleteObject(Brush);

		for (int i = 0; i < 6; i++)
		{
			for (int k = 0; k < 3; k++)
				HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/EmptyWaitroom.bmp")->Draw(75 + 220 * k, 80 + 75 * i);
		}

		break;
	}
	case WAITROOM:
	{
		HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/WaitingRoom.bmp")->Draw(50, 60);
		SetTextColor(HEngine::ResourceManager::GetInstance()->GetBackDC(), RGB(255, 255, 0));
		TextOut(HEngine::ResourceManager::GetInstance()->GetBackDC(), 215 + 50, 450 + 60, m_StrBuf, strlen(m_StrBuf));

		for (int i = 0; i < WAITROOM_PLAYER_MAX; i++)
		{
			if(m_sPlayerId[i] != "")
				HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/Bazzi.bmp")->Draw(85 + 165 * i, 60 + 53);

			RECT rect = { 50 + 37 + 163 * i, 146 + 60 , 50 + 178 + 162 * i,146 + 60 + 20 };
			DrawText(HEngine::ResourceManager::GetInstance()->GetBackDC(), m_sPlayerId[i].c_str(), strlen(m_sPlayerId[i].c_str()), &rect, DT_CENTER);
		}

		break;
	}
	case GAME_READY:
	{
		HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/BackGround.bmp")->Draw(0, 0);
		MapManager::GetInstance()->Draw();
		m_Player->Draw();

		HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/Ready.bmp")->Draw(110, 195);

		HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/Cursur2.bmp")->Draw(m_Player->GetDrawPT().x, m_Player->GetDrawPT().y - 40);

		break;
	}
	case GAME:
	{
		HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/BackGround.bmp")->Draw(0, 0);
		MapManager::GetInstance()->Draw();
		m_Player->Draw();
		BubbleManager::GetInstance()->Draw();

		if (m_fDuringTime <= 1.5f)
			HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/Start.bmp")->Draw(130, 250);
		break;
	}

	}
	HEngine::UIManager::GetInstance()->Draw();
}

bool GameManager::SetState(int state)
{
	HEngine::UIManager::GetInstance()->ReleaseUI();
	m_fDuringTime = 0.0f;
	m_iState = (GAME_STATE)state;
	return true;
}

bool GameManager::Update()
{
	if (!HEngine::TimeManager::GetInstance()->Update())
		return false;
	m_fDuringTime += HEngine::TimeManager::GetInstance()->GetElipseTime();

	HEngine::InputManager::GetInstance()->Update();

	HEngine::UIManager::GetInstance()->Update();

	if (m_iState == GAME_READY)
	{
		if (m_fDuringTime >= 3.0f)
			SetState(GAME);
	}
	if (m_iState == GAME)
	{
		HEngine::UIManager::GetInstance()->ReleaseUI();
		HEngine::UIManager::GetInstance()->AddLabel(m_sID, 705, 120, NULL, false);

		if (m_Player->GetState() != DIE && m_Player->GetState() != END)
		{
			HEngine::UIManager::GetInstance()->AddLabel(m_sID, m_Player->GetDrawPT().x, m_Player->GetDrawPT().y, NULL, false);

		}
		else
		{
			HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/Lose.bmp")->Draw(140, 50);
			HEngine::UIManager::GetInstance()->AddButton(200, 530, "./RES/BackButton.bmp", IntoWaitRoom, 0, FALSE);
		}

		MapManager::GetInstance()->Update();


		if (m_Player->Update())
		{
			PACKET_PLAYERMOVE packet;
			memcpy(packet.Id, m_sID.c_str(), m_sID.size());
			packet.Id[m_sID.size()] = 0;
			packet.BeforePt.x = m_Player->GetBackPt().x;
			packet.BeforePt.y = m_Player->GetBackPt().y;
			packet.PlayerPt.x = m_Player->GetPt().x;
			packet.PlayerPt.y = m_Player->GetPt().y;
			packet.header.wIndex = PACKET_INDEX_PLAYER;
			packet.header.wLen = sizeof(PACKET_HEADER) + sizeof(POINT) * 2 + sizeof(char)*(strlen(packet.Id) + 1) + sizeof(int)*2;
			packet.RoomNO = m_iRoomNo;
			packet.Speed = m_Player->GetSpeed();
			send(m_ServerSock, (char *)&packet, packet.header.wLen, 0);
		}
		

		if (HEngine::InputManager::GetInstance()->isKeyDown(VK_SPACE) && MapManager::GetInstance()->GetType(m_Player->GetPt()) != BUBBLE)
		{
			if (BubbleManager::GetInstance()->AddBubble(m_Player->GetPt(), m_Player->GetPower(), true))
			{
				PACKET_BUBBLE packet;
				packet.header.wIndex = PACKET_INDEX_BUBBLE;
				packet.header.wLen = sizeof(PACKET_HEADER) + sizeof(POINT) + sizeof(int) * 2;
				packet.pt.x = m_Player->GetPt().x;
				packet.pt.y = m_Player->GetPt().y;
				packet.RoomNo = m_iRoomNo;
				packet.Power = m_Player->GetPower();
				send(m_ServerSock, (char *)&packet, packet.header.wLen, 0);
			}
		}

		if (MapManager::GetInstance()->GetType(m_Player->GetPt()) >= BOOM_CENTER && m_Player->GetState() <= DIEBUBBLE)
		{
			PACKET_PLAYERSTATE packet;
			packet.header.wIndex = PACKET_INDEX_PLAYERSTATE;
			memcpy(packet.Id, m_sID.c_str(), m_sID.size());
			packet.state = DIEBUBBLE;
			packet.Id[m_sID.size()] = 0;
			packet.RoomNo = m_iRoomNo;
			packet.header.wLen = sizeof(PACKET_HEADER) + sizeof(char)*(strlen(packet.Id) + 1) + sizeof(int) * 2;

			m_Player->SetState(DIEBUBBLE);
			send(m_ServerSock, (char*)&packet, packet.header.wLen, 0);
		}

		else if (m_Player->GetState() == DIE)
		{
			PACKET_PLAYERSTATE packet;
			packet.header.wIndex = PACKET_INDEX_DIE;
			packet.header.wLen = sizeof(PACKET_PLAYERSTATE);
			memcpy(packet.Id, m_sID.c_str(), sizeof(packet.Id));
			packet.Id[m_sID.size()] = 0;
			packet.RoomNo = m_iRoomNo;
			packet.state = DIE;
			send(m_ServerSock, (char *)&packet, packet.header.wLen, 0);
			m_Player->SetState(END);
		}
		if (MapManager::GetInstance()->CheckWin() && m_Player->GetState() != END)
		{
			HEngine::ResourceManager::GetInstance()->GetBitmap("./RES/Win.bmp")->Draw(140, 50);
			HEngine::UIManager::GetInstance()->AddButton(200, 530, "./RES/BackButton.bmp", IntoWaitRoom, 0, FALSE);
		}
		BubbleManager::GetInstance()->Update();
	}

	//Draw();
	HDC hdc = GetDC(m_hWnd);
	HEngine::ResourceManager::GetInstance()->DrawBack(hdc);
	Draw();
	ReleaseDC(m_hWnd, hdc);
	return false;
}


void GameManager::Typing(WPARAM wParam)
{
	int len;
	len = strlen(m_StrBuf);

	if (GetAsyncKeyState(VK_BACK))
	{
		m_StrBuf[--len] = 0;
		if (m_StrBuf[len - 1] < 0)
			m_StrBuf[--len] = 0;
		return;
	}

	if (GetAsyncKeyState(VK_RETURN))
	{
		if (strlen(m_StrBuf) <= 0)
			return;
		else if (m_iState == LOGIN)
		{
			SetLoginRT(0);
		}
		else if (m_iState == WAITROOM)
		{
			string str = "[" + m_sID + "] : ";
			str += m_StrBuf;

			PACKET_TEXT packet;
			packet.header.wIndex = PACKET_INDEX_TEXT;

			memcpy(packet.wStr, &str.c_str()[0], str.size());
			packet.wStr[str.size()] = 0;
			packet.RoomNo = m_iRoomNo;
			packet.Count = str.size();
			packet.header.wLen = sizeof(PACKET_HEADER) + sizeof(packet.wStr) + sizeof(int) + sizeof(int);
			packet.RoomNo = m_iRoomNo;
			send(m_ServerSock, (char *)&packet, packet.header.wLen, 0);
		}
		ZeroMemory(m_StrBuf, sizeof(m_StrBuf));

	}
	else
	{
		if (m_iState == LOGIN && len >= MAX_ID_SIZE)
			return;
		if (m_iState == WAITROOM && len >= 55)
			return;
		m_StrBuf[len++] = (TCHAR)wParam;
		m_StrBuf[len] = 0;
	}
}

string GameManager::GetID()
{
	if (m_sID == "")
	{
		m_sID = m_StrBuf;
		ZeroMemory(m_StrBuf,sizeof(m_StrBuf));
	}

	return m_sID;
}

SOCKET GameManager::GetServerSock()
{
	return m_ServerSock;
}

void GameManager::SetRoomNo(int RoomNo)
{
	m_iRoomNo = RoomNo;
	ZeroMemory(m_StrBuf, sizeof(m_StrBuf));
	m_iMessageLine = 0;
}



void GameManager::ProcessSocketMessage(HWND hWnd, int iMessage, WPARAM wParam, LPARAM lParam, SOCKET g_Sock)
{
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen = 0;
	int retval = 0;
	m_ServerSock = g_Sock;
	if (WSAGETSELECTERROR(lParam))
	{
		int err_code = WSAGETSELECTERROR(lParam);
		return;
	}

	switch (WSAGETSELECTEVENT(lParam))
	{
	case FD_READ:
	{
		char szBuf[BUFSIZE];
		retval = recv(wParam, szBuf, BUFSIZ, 0);

		memcpy(&m_Buf[g_iLen], szBuf, retval);

		g_iLen += retval;

		if (retval == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				exit(1);
				//cout << "err on recv!!" << endl;
			}
		}

		if (!ProcessPacket(&m_Buf[0], retval))
		{
			Sleep(100);
			SendMessage(hWnd, iMessage, wParam, lParam);
		}
		break;
	}
	case FD_CLOSE:
	break;
	}
}

bool GameManager::ProcessPacket(char * szBuf, int retval)
{
	if (g_iLen < sizeof(PACKET_HEADER))
		return false;

	PACKET_HEADER header;
	memcpy(&header, szBuf, sizeof(header));

	if (g_iLen < header.wLen)
		return false;

	switch (header.wIndex)
	{
	case PACKET_INDEX_LOGINRT:
	{
		m_sID = "";
		MessageBox(m_hWnd, "이미 아이디가 존재 합니다.\n다른 아이디를 사용해 주세요\n", "경고 !", MB_OK);
	}
		break;
	case PACKET_INDEX_LOADWAITROOM: //(대기실 목록 출력)
	{
		if (m_sID == "")
			break;
		SetState(LOGINRT);
		PACKET_WAITROOMS packet;
		memcpy(&packet, szBuf, header.wLen);

		HEngine::UIManager::GetInstance()->ReleaseUI();
		int i = 0;
		for (; i < packet.iFullLen; i++)
		{
			if (i >= 18)
				break;
			HEngine::UIManager::GetInstance()->AddButton(75 + 220 * (i / 6), 80 + 75 * (i % 6), "./RES/WaitRoom.bmp", IntoWaitRoom, packet.iFull[i], false);
			HEngine::UIManager::GetInstance()->AddLabel(to_string(packet.iFull[i]), 65 + 75 + 220 * (i / 6), 12 + 80 + 75 * (i % 6), NULL, false);

		}

		HEngine::UIManager::GetInstance()->AddButton(50, 10, "./RES/MakeRoom.bmp", IntoWaitRoom, packet.iLastRoomNo, false);

		break;
	}
	case PACKET_INDEX_WAITROOM:
	{
		PACKET_WAITROOM packet;
		memcpy(&packet, szBuf, header.wLen);
		SetState(WAITROOM);
		SetRoomNo(packet.RoomNo);
		HEngine::UIManager::GetInstance()->AddButton(800 - 145 - 50, 30, "./RES/BackButton.bmp", SetLoginRT, 0, false);
		HEngine::UIManager::GetInstance()->AddButton(640, 560, "./RES/GameReadyButton.bmp", SetGameStart, 0, false);
		break;
	}
	case PACKET_INDEX_PLAYERINFO:
	{
		PACKET_PLAYERINFO packet;
		memcpy(&packet, szBuf, header.wLen);
		for (int i = 0; i < WAITROOM_PLAYER_MAX; i++)
		{
			m_sPlayerId[i] = packet.Id[i];
			//memcpy(m_sPlayerId[i], packet.Id[i], strlen(packet.Id[i]));
		}
		m_iWaitingRoomPlayer = packet.PlayerCount;

		break;
	}
	case PACKET_INDEX_GAMESTART:
	{
		PACKET_GAMESTART packet;
		memcpy(&packet, szBuf, header.wLen);

		BubbleManager::GetInstance()->Release();

		MapManager::GetInstance()->Init(packet);

		m_Player->Init();
		
		for (int i = 0; i < packet.PlayerCount; i++)
		{
			HEngine::POINT pt;
			pt.x = MapManager::GetInstance()->GetInitPt(i).x;
			pt.y = MapManager::GetInstance()->GetInitPt(i).y;
			if (m_sPlayerId[i] == m_sID)
				m_Player->SetPt(pt);

			else
				MapManager::GetInstance()->AddPlayer(pt, m_sPlayerId[i]);
		}
		GameManager::GetInstance()->SetState(GAME_READY);

		break;
	}
	case PACKET_INDEX_PLAYER:
	{
		PACKET_PLAYERMOVE packet;
		memcpy(&packet, szBuf, header.wLen);
		if (m_sID == packet.Id)
			break;

		HEngine::POINT Beforept;
		Beforept.x = packet.BeforePt.x;
		Beforept.y = packet.BeforePt.y;
		HEngine::POINT Afterpt;

		Afterpt.x = packet.PlayerPt.x;
		Afterpt.y = packet.PlayerPt.y;
		MapManager::GetInstance()->MovePlayer(Beforept, Afterpt, packet.Id, packet.Speed);
		break;
	}
	case PACKET_INDEX_BUBBLE:
	{
		PACKET_BUBBLE packet;
		memcpy(&packet, szBuf, header.wLen);
		MapManager::GetInstance()->AddType({ packet.pt.x, packet.pt.y }, BUBBLE);
		BubbleManager::GetInstance()->AddBubble({ packet.pt.x, packet.pt.y }, packet.Power, false);
		break;
	}
	case PACKET_INDEX_PLAYERSTATE:
	{
		PACKET_PLAYERSTATE packet;
		memcpy(&packet, szBuf, header.wLen);
		MapManager::GetInstance()->SetPlayerState(packet.Id, (DIRECT)packet.state);
		break;
	}
	case PACKET_INDEX_TEXT:
	{
		PACKET_TEXT packet;
		memcpy(&packet, szBuf, header.wLen);
		if (HEngine::UIManager::GetInstance()->AddLabel(packet.wStr, 30 + 50, 350 + 60 + 15 * m_iMessageLine, DT_WORDBREAK, false))
			m_iMessageLine++;

		break;
	}

	case PACKET_INDEX_DIE:
	{
		PACKET_PLAYERSTATE packet;
		memcpy(&packet, szBuf, header.wLen);
		MapManager::GetInstance()->SetPlayerState(packet.Id, DIE);
		
		break;
	}
	case PACKET_INDEX_EXIT:
	{
		PACKET_LOGIN packet;
		memcpy(&packet, szBuf, header.wLen);
		packet.ID[packet.validID] = 0;
		if (m_iState == GAME || m_iState == GAME_READY)
		{
			MapManager::GetInstance()->SetPlayerState(packet.ID, DIE);
		}
		break;
	}
	}
	memcpy(&m_Buf[0], &m_Buf[header.wLen], sizeof(m_Buf) - g_iLen);
	g_iLen -= header.wLen;

	if (g_iLen >= sizeof(PACKET_HEADER))
		return ProcessPacket(&m_Buf[0], g_iLen);
	return true;
}

void GameManager::Release()
{
	PACKET_WAITROOM packet;
	packet.header.wIndex = PACKET_INDEX_EXIT;
	packet.header.wLen = sizeof(PACKET_HEADER) + sizeof(int);
	packet.RoomNo = m_iRoomNo;
	send(m_ServerSock, (char *)&packet, packet.header.wLen, 0);

	MapManager::GetInstance()->Release();
	BubbleManager::GetInstance()->Release();
	HEngine::UIManager::GetInstance()->ReleaseUI();

	SAFE_DELETE(m_Player);
		

	MapManager::GetInstance()->DestroyInstance();
	BubbleManager::GetInstance()->DestroyInstance();
	HEngine::ResourceManager::GetInstance()->DestroyInstance();
	HEngine::InputManager::GetInstance()->DestroyInstance();
	HEngine::TimeManager::GetInstance()->DestroyInstance();
	HEngine::UIManager::GetInstance()->DestroyInstance();
}

int GameManager::GetRoomNo()
{
	return m_iRoomNo;
}

GameManager::GameManager()
{
}


GameManager::~GameManager()
{
	
}
