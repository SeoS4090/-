#include "Defines.h"
#include "ClientManager.h"
#include "ServerManager.h"
#include "WaitRoomManager.h"
//소켓 정보 저장을 위한 구조체


DWORD WINAPI WorkerThread(LPVOID arg);

int main(int argc, char *argv[])
{
	system("title Demo_CrazyAcade Server");
	printf("\t\t크레이지 아케이드 데모 서버 입니다. \n");

	int retval;

	ServerManager::GetInstance()->Init();

	//입출력 완료 포트 생성
	HANDLE hcp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (hcp == NULL) return 1;

	//cpu 개수 확인
	SYSTEM_INFO si;
	GetSystemInfo(&si);

	//(CPU 개수 * 2)개의 작업자 스레드 생성
	HANDLE hThread;
	for (int i = 0; i < (int)si.dwNumberOfProcessors * 2; i++) {
		hThread = CreateThread(NULL, 0, WorkerThread, hcp, 0, NULL);
		if (hThread == NULL) return 1;
		CloseHandle(hThread);
	}
	
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	DWORD recvbytes, flags;

	while (1)
	{
		
		if (ServerManager::GetInstance()->Accept(client_sock, clientaddr, addrlen))
			break;

		printf("[TCP 서버] 클라이언트 접속 : IP 주소 = %s , 포트번호 = %d,  소켓번호 = %d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port), client_sock);
		
		
		//소켓과 입출력 완료 포트 연결
		CreateIoCompletionPort((HANDLE)client_sock, hcp, client_sock, 0);

		ClientManager::GetInstance()->addClient(client_sock, clientaddr);

		SOCKETINFO * ptr = ClientManager::GetInstance()->GetSocketInfo(client_sock);

		flags = 0;
		retval = WSARecv(client_sock, &ptr->wsabuf, 1, &recvbytes, &flags, &ptr->overlapped, NULL);
		if (retval == SOCKET_ERROR)
		{
			if (WSAGetLastError() != ERROR_IO_PENDING)
				printf("WSARecv()\n");
			continue;
		}
	}

	ServerManager::GetInstance()->Release();
	return 0;
}

DWORD WINAPI WorkerThread(LPVOID arg)
{
	int retval;
	HANDLE hcp = (HANDLE)arg;

	while (1) {
		//비동기 입출력 완료 기다리기
		DWORD cbTransferred;
		SOCKET client_sock;
		SOCKETINFO *ptr;
		retval = GetQueuedCompletionStatus(hcp, &cbTransferred, (LPDWORD)&client_sock, (LPOVERLAPPED*)&ptr, INFINITE);

		//클라이언트 정보 얻기
		SOCKADDR_IN clientaddr;
		int addrlen = sizeof(clientaddr);
		getpeername(ptr->sock, (SOCKADDR *)&clientaddr, &addrlen);

		//비동기 입출력 결과 확인
		if (retval == 0 || cbTransferred == 0) {
			if (retval == 0) {
				DWORD temp1, temp2;
				WSAGetOverlappedResult(ptr->sock, &ptr->overlapped, &temp1, FALSE, &temp2);
				//err_display("WSAGetOverlappedResult()");
			}
			closesocket(ptr->sock);
			printf("[TCP 서버] 클라이언트 종료: IP 주소 = %s, 포트 번호 = %d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
			continue;
		}


		ptr->recvbytes += cbTransferred;

		ServerManager::GetInstance()->ProcessPacket(ptr);
			
		DWORD recvbytes;
		DWORD flags = 0;
		retval = WSARecv(ptr->sock, &ptr->wsabuf, 1, &recvbytes, &flags, &ptr->overlapped, NULL);
		
	}
	return 0;
}	