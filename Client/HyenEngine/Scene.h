#pragma once
#include "GlobalDefine.h"

namespace HEngine
{
	class Scene
	{
	public:
		Scene();

		virtual ~Scene();

		virtual void Init(HWND hWnd) = 0;
		virtual bool Input(float fEtime) = 0;
		virtual void Draw(HDC hdc) = 0;
		virtual void Update(float fEtime) = 0;
		virtual void Release() = 0;
	};
}