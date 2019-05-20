#pragma once
#include "GlobalDefine.h"
#include "POINT.h"
namespace HEngine
{
	enum UITYPE
	{
		LABEL,
		BUTTON
	};

	class UIObject
	{
	protected:
		string UIName;
		HEngine::RECT m_RecPos;
		void SetPos(int left, int top , int right, int bottom);

	public:
		UIObject();
		virtual ~UIObject();
		virtual void Draw() = 0;
		virtual bool Update() = 0;
		virtual UITYPE GetType() = 0;
		string GetString();
		void SetString(string s);
	};


}