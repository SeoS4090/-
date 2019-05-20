#pragma once
#include <Windows.h>
#include <string>
#include <vector>
#include <functional>

using namespace std;

typedef std::function<bool()> pBoolFunc;

#define SAFE_DELETE( p ) { if( p ) { delete ( p ); ( p ) = NULL; } }
#define SAFE_DELETE_ARRAY( p ) { if( p ) { delete[] ( p ); ( p ) = NULL; } }
#define SAFE_RELEASE(p) { if ( (p) ) { (p)->Release(); (p) = 0; } }