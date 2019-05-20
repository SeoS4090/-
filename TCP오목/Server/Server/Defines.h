#pragma once
#include <Windows.h>
#include <iostream>
#include <string>

#define BUFSIZE		512
#define WM_SOCKET	(WM_USER+1)
#define SERVERPORT	9000
#define SAFE_DELETE( p ) { if( p ) { delete ( p ); ( p ) = NULL; } }
#define SAFE_DELETE_ARRAY( p ) { if( p ) { delete[] ( p ); ( p ) = NULL; } }
#define SAFE_RELEASE(p) { if ( (p) ) { (p)->Release(); (p) = 0; } }
using namespace std;

