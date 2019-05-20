#pragma once
#include "GlobalDefine.h"
#include "POINT.h"
#define HWND_WIDTH 514
#define HWND_HEIGHT 800
#define GAME_WIDTH 514
#define GAME_HEIGHT_START 115
#define GAME_HEIGHT_END 750
#define AIRPLAN_SPEED 200
#define BULLET_RESPAWN_TIME 0.2f
#define BULLET_MAX_COUNT 50
#define SCORE_MAX_COUNT 5
#define SCORE_RESPAWN_TIME 0.2f
#define GAME_MAXTIME "45"

struct Paper_Position
{
	HEngine::POINTF Paper_RIGHT =	{ 195 + 169,365 };
	HEngine::POINTF Paper_LEFT	=	{ 195 - 171,365 };
	HEngine::POINTF Paper_UP	=	{ 195,365 - 182 };
	HEngine::POINTF Paper_DOWN	=	{ 195,365 + 182 };
	HEngine::POINTF Paper_CENTER =	{ 195,365 };
};

enum SCENE_INDEX
{
	SCENE_TITLE, 
	SCENE_SELECTGAME,
	SCENE_COLORPAPER,
	SCENE_AIRPLAN
};

enum GAME_STATE
{
	STATE_PAUSE,
	STATE_GAME,
	STATE_END
};

enum OBJECT_TYPE
{
	OBJECT,
	OBJECT_BULLET,
	OBJECT_STAR,
	OBJECT_PAPER
};

enum DIRECT
{
	UP,
	DOWN,
	RIGHT,
	LEFT,
	UPLEFT,
	UPRIGHT,
	DOWNLEFT,
	DOWNRIGHT
};
enum PAPER
{
	RED,
	GREEN,
	BLUE,
	YELLOW
};

struct ColliderType
{
	OBJECT_TYPE type;
	bool m_bCollider;
};