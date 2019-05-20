#pragma once
#include <Windows.h>
#include <string>
#include <vector>
#include <functional>
#include <imm.h>
#include "../../Common/PACKET.h"

#define WM_SOCKET				(WM_USER+1)
#define BUFSIZE					1024
#define SERVERPORT				9000
#define SERVERIP				"127.0.0.1"
#define WAITROOM_PLAYER_MAX		8

#define BUBBLE_BOOM_TIME		3.5f
#define BUBBLE_BOOM_DURINGTIME	0.5f
#define ITEM_SPEED				30
#define MAX_SPEED				600
using namespace std;

typedef std::function<bool(int)> pBoolFunc;

#define SAFE_DELETE( p ) { if( p ) { delete ( p ); ( p ) = NULL; } }
#define SAFE_DELETE_ARRAY( p ) { if( p ) { delete[] ( p ); ( p ) = NULL; } }
#define SAFE_RELEASE(p) { if ( (p) ) { (p)->Release(); (p) = 0; } }

enum DIRECT
{
	UP,
	DOWN,
	RIGHT,
	LEFT,
	DIEBUBBLE,
	DIE,
	END
};

