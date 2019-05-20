#include "GameManager.h"
#include "UIManager.h"
#include "Mecro.h"
#include <string>

GameManager* GameManager:: pThis = NULL;
bool IntoWaitRoom(int arg)
{
	UIManager::GetInstance()->Release();
	GameManager::GetInstance()->SetWaitRoom(arg);
	GameManager::GetInstance()->SetState(WAIT_ROOM);
	return true;
}
bool BackButton(int State)
{
	GameManager::GetInstance()->ClearMap();
	PACKET_HEADER packet;
	packet.wIndex = PACKET_INDEX_RETURN;
	packet.wLen = sizeof(PACKET_HEADER);
	send(GameManager::GetInstance()->GetServerSock(), (char *)&packet, sizeof(packet), 0);
	GameManager::GetInstance()->SetState(GAME_STATE(State));
	return true;
}

bool GameStart(int RoomNo)
{
	UIManager::GetInstance()->Release();
	GameManager::GetInstance()->ClearMap();

	if (GameManager::GetInstance()->GetState() == GAME_READY)
	{
		GameManager::GetInstance()->SetState(GAME_START);
	}
	else 
		GameManager::GetInstance()->SetState(GAME_READY);
	return true;
}
void GameManager::SetState(GAME_STATE state)
{
	Sleep(10);
	UIManager::GetInstance()->Release();
	m_iState = state;
	ClearMap();

	switch (m_iState)
	{
	case LOGIN:
	{
		m_iMessageLine = 0;

		PACKET_HEADER packet;
		packet.wIndex = PACKET_INDEX_LOGIN;
		packet.wLen = sizeof(PACKET_HEADER);
		send(ServerSock, (char *)&packet, sizeof(packet),0);
	}
		break;

	case WAIT_ROOM:
	{
		PACKET_WAITROOM packet;
		packet.header.wIndex = PACKET_INDEX_WAITROOM;
		packet.header.wLen = sizeof(PACKET_HEADER) + sizeof(int);
		packet.iRoomNo = m_iWaitRoom;
		send(ServerSock, (char *)&packet, sizeof(packet), 0);

		UIManager::GetInstance()->AddButton(m_hdc, "Back", { 560,25 }, BackButton, LOGIN);

	}
		break;
	case GAME_READY:
	{
		PACKET_HEADER packet;
		packet.wIndex = PACKET_INDEX_GAMEREADY;
		packet.wLen = sizeof(PACKET_HEADER);
		send(ServerSock, (char *)&packet, sizeof(packet), 0);

		UIManager::GetInstance()->AddButton(m_hdc, "Back", { 560,25 }, BackButton, LOGIN);

	}
		break;
	case GAME_START:
	{
		PACKET_HEADER packet;
		packet.wIndex = PACKET_INDEX_GAMESTART;
		packet.wLen = sizeof(PACKET_HEADER);
		send(ServerSock, (char *)&packet, sizeof(packet), 0);

		UIManager::GetInstance()->AddButton(m_hdc, "Back", { 610,25 }, BackButton, LOGIN);
	}
		break;

	case WIN:
		Draw(m_hdc);
		break;
	case LOSE:
		Draw(m_hdc);
		break;
	}
}
void GameManager::SetWaitRoom(int RoomNo)
{
	m_iWaitRoom = RoomNo;
}
void GameManager::Init(HWND hWnd)
{
	m_hdc = GetDC(hWnd);
	pBoolFunc func = IntoWaitRoom;
	m_iMessageLine = 0;
	SetState(LOGIN);
}

void GameManager::SetTurn()
{
	if (m_bTrun)
		m_bTrun = false;
	else
		m_bTrun = true;
}

void GameManager::Message(WPARAM wParam)
{
	if (m_iState == LOGIN)
		return;
	
	len = strlen(str);
	if (GetAsyncKeyState(VK_BACK))
	{
		if (len > 0)
			str[len - 1] = 0;
		return;
	}
	if (GetAsyncKeyState(VK_RETURN))
	{
		if (strlen(str) <= 0)
			return;
		PACKET_TEXT packet;
		packet.header.wIndex = PACKET_INDEX_TEXT;
		
		memcpy(packet.wStr, str, sizeof(str));

		packet.wStr[strlen(str)] = 0;
		packet.header.wLen = sizeof(PACKET_HEADER) + sizeof(packet.wStr);
		send(ServerSock, (char *)&packet, sizeof(packet), 0);
		
		string s = "나 : ";
		s.append(str);
		if(m_iState == GAME_START)
		{
			if (UIManager::GetInstance()->AddLabel(m_hdc, s, { 610,550 + 25 * m_iMessageLine }))
				m_iMessageLine++;
		}
		else
		{
			if (UIManager::GetInstance()->AddLabel(m_hdc, s, { 15,30 + 25 * m_iMessageLine }))
				m_iMessageLine++;
		}	

		for (int i = 0; i < len; i++)
		{
			str[i] = 0;
		}

	}
	else
	{
		str[len] = (TCHAR)wParam;
		str[len + 1] = 0;
	}
}

void DrawMap(HDC hdc)
{
	for (int i = 0; i < 12; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			Rectangle(hdc, i * 50 + 10, j * 50 + 25, i * 50 + 60, j * 50 + 75);
		}
	}
}

void GameManager::Draw(HDC hdc)
{
	Rectangle(hdc, 10, 25, 560, 575);
	if (m_iState == LOGIN)
	{
		TextOut(hdc, 15, 5, "welcome to the Omok Land", sizeof("welcome to the Omok Land")-1);
	}
	if (m_iState == WAIT_ROOM || m_iState== GAME_READY)
	{
		TextOut(hdc, 15, 5, "대 기 실" , sizeof("대 기 실") - 1 );
		TextOut(hdc, 15, 555, str, strlen(str));
	}

	if (m_iState == GAME_START || m_iState == WIN || m_iState == LOSE)
	{
		DrawMap(hdc);
		TextOut(hdc, 610, 610, str, strlen(str));

		for (int Height = 0; Height < MAPSIZE; Height++)
		{
			for (int Width = 0; Width < MAPSIZE; Width++)
			{
				if (m_Map[Width][Height] == true)
				{
					Ellipse(hdc, Width * 50 + 10 - 25, Height * 50 , Width * 50 + 10 + 25, Height * 50 + 50);
				}
				else if (m_Map[Width][Height] == false)
				{
					HBRUSH Brush = CreateSolidBrush(RGB(0, 0, 0));
					HBRUSH oBrush = (HBRUSH)SelectObject(hdc, Brush);
					Ellipse(hdc, Width * 50 + 10 - 25, Height * 50, Width * 50 + 10 + 25, Height * 50 + 50);
					SelectObject(hdc, oBrush);
					DeleteObject(Brush);
				}
			}
		}
	}

	UIManager::GetInstance()->Draw(hdc);
}

void GameManager::GameRestart()
{
	m_iState = WAIT_ROOM;
	UIManager::GetInstance()->Release();
	UIManager::GetInstance()->AddButton(m_hdc, "Back", { 560,25 }, BackButton, LOGIN);
	UIManager::GetInstance()->AddButton(m_hdc, "게임 레디", { 560,555, }, GameStart, 0);
	m_iMessageLine = 0;

	for (int i = 0; i < 256; i++)
	{
		str[i] = 0;
	}
	ClearMap();

}

bool GameManager::AddStone(int x, int y)
{
	if (!m_bTrun)
		return false;
	x = (int)x / 50;
	y = (int)y / 50;

	if (y > 12 || x > 12)
	{
		return false;
	}
	
	if (m_Map[x][y] != 3)
		return false;

	m_Map[x][y] = m_bTeam;
	PACKET_STONE packet;
	packet.header.wIndex = PACKET_INDEX_PLAYER;
	packet.header.wLen = sizeof(PACKET_HEADER) + sizeof(WORD)+ sizeof(WORD);

	packet.x = x;
	packet.y = y;
	
	send(ServerSock, (char *)&packet, sizeof(packet), 0);
	SetTurn();

	if (GameManager::GetInstance()->CheckWin(m_bTeam))
	{
		GameManager::GetInstance()->SetState(WIN);
	}
	return true;
}

void GameManager::Release()
{
	m_bTeam = true;
	m_bTrun = false;
	g_iLen = 0;
	m_iState = LOGIN;
	m_iMessageLine = 0;
	for (int i = 0; i < MAPSIZE; i++)
	{
		for (int j = 0; j < MAPSIZE; j++)
		{
			m_Map[i][j] = 3;
		}
	}

	PACKET_HEADER packet;
	packet.wIndex = PACKET_INDEX_EXIT;
	packet.wLen = sizeof(PACKET_HEADER);
	send(ServerSock, (char *)&packet, sizeof(packet), 0);
	UIManager::GetInstance()->Destroy();
	SAFE_DELETE(pThis);
}

void GameManager::ClearMap()
{
	for (int i = 0; i < MAPSIZE; i++)
	{
		for (int j = 0; j < MAPSIZE; j++)
		{
			m_Map[i][j] = 3;
		}
	}
}
GameManager::GameManager()
{
	m_bTeam = true;
	m_bTrun = false;
	g_iLen = 0;
	g_iIndex = -1;
	m_iState = NULL;
	m_iMessageLine = 0;

	for (int i = 0; i < 256; i++)
	{
		str[i] = 0;
	}
	ClearMap();
}

GameManager::~GameManager()
{
}

bool GameManager::CheckWin(bool Team)
{
	for (int i = 0; i < MAPSIZE; i++) //HEIGHT 세로
	{
		for (int j = 0; j < MAPSIZE; j++) //WIDTH 가로
		{
			bool bRet = true;
			for (int k = 0; k < 5; k++)
			{
				int index = j + k; //가로 방향
				if (index >= MAPSIZE)
				{
					bRet = false;
					break;
				}

				if (m_Map[i][index] != Team)
				{
					bRet = false;
					break;
				}
			}

			if (bRet)
				return true;

			bRet = true;

			for (int k = 0; k < 5; k++)
			{
				int index = i + k; //세로 방향
				if (index >= MAPSIZE)
				{
					bRet = false;
					break;
				}

				if (m_Map[index][j] != Team)
				{
					bRet = false;
					break;
				}
			}

			if (bRet)
				return true;
			bRet = true;

			for (int k = 0; k < 5; k++)
			{
				int index_i = i + k; //세로 + 1
				if (index_i >= MAPSIZE)
				{
					bRet = false;
					break;
				}

				int index_j = j + k; //가로 +1 = 좌하 대각
				if (index_j >= MAPSIZE)
				{
					bRet = false;
					break;
				}

				if (m_Map[index_i][index_j] != Team)
				{
					bRet = false;
					break;
				}
			}

			if (bRet)
				return true;
			bRet = true;

			for (int k = 0; k < 5; k++)
			{
				if (j == 5)
					j = 5;
				int index_i = i + k; //세로 + 1
				if (index_i >= MAPSIZE)
				{
					bRet = false;
					break;
				}

				int index_j = j - k; //가로 -1 = 우좌하
				if (index_j >= MAPSIZE)
				{
					bRet = false;
					break;
				}

				if (m_Map[index_i][index_j] != Team)
				{
					bRet = false;
					break;
				}
			}

			if (bRet)
				return true;
		}
	}
	return false;
}

void GameManager::ProcessSocketMessage(HWND hWnd, int iMessage, WPARAM wParam, LPARAM lParam , SOCKET g_Sock)
{
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen = 0;
	int retval = 0;
	ServerSock = g_Sock;
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
	case FD_CLOSE:
	{
	}
	break;
	}
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
	case PACKET_INDEX_LOADWAITROOM: //(대기실 목록 출력)
	{
		PACKET_WAITROOMS packet;
		memcpy(&packet, szBuf, header.wLen);

		if (m_iState == LOGIN)
		{
			UIManager::GetInstance()->Release();
			int i = 0;
			for (; i < packet.iFullLen; i++)
			{
				UIManager::GetInstance()->AddButton(m_hdc, string(to_string(packet.iFull[i] + 1) + "번방"), { 10,25 + 25 * i }, IntoWaitRoom, packet.iFull[i]);
			}

			if(packet.iFullLen > 0)
				UIManager::GetInstance()->AddButton(m_hdc, "대기실 만들기", { 560,25 }, IntoWaitRoom, packet.iFull[i] + 1);
			else 
				UIManager::GetInstance()->AddButton(m_hdc, "대기실 만들기", { 560,25 }, IntoWaitRoom, 0);

		}
		g_iLen -= header.wLen;
		break;
	}
	case PACKET_INDEX_WAITROOM:
	{
		if (UIManager::GetInstance()->AddLabel(m_hdc, "상대방이 로그인 하였습니다.", { 15,30 + 30 * m_iMessageLine }))
			m_iMessageLine++;
		UIManager::GetInstance()->AddButton(m_hdc, "게임 레디", { 560,555, }, GameStart, 0);
		g_iLen -= header.wLen;
		break;
	}

	case PACKET_INDEX_GAMEREADY:
	{
		if (UIManager::GetInstance()->AddLabel(m_hdc, "게임 레디 버튼을 눌러 주세요.", { 15,30 + 30 * m_iMessageLine }))
			m_iMessageLine++;
		m_iState = GAME_READY;
		m_bTeam = true;
		m_bTrun = false;

		g_iLen -= header.wLen;
		break;
	}
	//PACKET_INDEX_TEXT (대기실일경우 메세지 상대방에게 넘기기),
	case PACKET_INDEX_TEXT:
	{
		PACKET_TEXT packet;
		memcpy(&packet, szBuf, sizeof(packet));
		string s = "상대방 : ";

		s.append(packet.wStr);
			if(m_iState == GAME_START)
		{
			if (UIManager::GetInstance()->AddLabel(m_hdc, s, { 610,550 + 25 * m_iMessageLine }))
				m_iMessageLine++;
		}
		else
		{
			if (UIManager::GetInstance()->AddLabel(m_hdc, s, { 15,30 + 25 * m_iMessageLine }))
				m_iMessageLine++;
		}	
		g_iLen -= sizeof(packet);
		break;
	}


	case PACKET_INDEX_GAMESTART:
	{
		m_iState = GAME_START;
		
		UIManager::GetInstance()->Release();
		UIManager::GetInstance()->AddButton(m_hdc, "Back", { 610,25 }, BackButton, LOGIN);
		m_bTeam = false;
		m_bTrun = true;
		g_iLen -= sizeof(header);

		break;
	}

	case PACKET_INDEX_PLAYER:
	{
		PACKET_STONE packet;
		memcpy(&packet, szBuf, sizeof(packet));

		m_Map[packet.x][packet.y] = !m_bTeam;

		if (CheckWin(!m_bTeam))
			GameManager::GetInstance()->SetState(LOSE);

		g_iLen -= sizeof(packet);
		SetTurn();
		break;
	}

	case PACKET_INDEX_RETURN:
	{
		m_iState = WAIT_ROOM;
		UIManager::GetInstance()->Release();
		UIManager::GetInstance()->AddButton(m_hdc, "Back", { 560,25 }, BackButton, LOGIN);
		m_iMessageLine = 0;
		ClearMap();
		g_iLen -= sizeof(header);
		break;
	}
	
	}
	memcpy(&m_Buf[0], &m_Buf[g_iLen], sizeof(m_Buf) - g_iLen);

	return true;
}
