#pragma once
#include "GlobalDefine.h"
#include "POINT.h"
namespace HEngine
{
	class UIObject
	{
	protected:
		HEngine::RECT m_RecPos;
		void SetPos(int left, int top , int right, int bottom);
	public:
		UIObject();
		virtual ~UIObject();
		
		virtual void Draw() = 0;
		virtual bool Update() = 0;
	};

}