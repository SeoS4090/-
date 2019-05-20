#pragma once
#include "GlobalDefine.h"
#include "POINT.h"
#include "SingleTone.h"

namespace HEngine
{
	/**
	* @brief ������ Input�� ����ϴ� Ŭ����
	* @details Ű�Է��� ����ϴ� Ŭ�����Դϴ�. RegistKeyCode�� ����� isKeyPress , isKeyUp , isKeyDown �Լ��� ���¸� �����մϴ�.
	* @author JBC classicaa@naver.com
	* @date 2016-10-14
	* @version 0.0.1
	*/

	class InputManager : public SingleTone<InputManager>
	{
		struct KEY_INFO
		{
		public:
			int				keyCode;
			bool			KeyPrecce;
			bool			keyUp;
			bool			KeyDown;
		};

	private:
		HWND							m_hWnd;
		HEngine::POINT					m_ptMouse;
		std::vector<InputManager::KEY_INFO>	m_vecKeyRegist;
		int CXFrame;
		int CYFrame;
	public:
		bool isExistKey(int key);
		void RegistKeyCode(int key);

		void Init(HWND hWnd);
		void Update();
		void Clear();

		HEngine::POINT GetMousePoint();
		bool isKeyPress(int keyCode);
		bool isKeyUp(int keyCode);
		bool isKeyDown(int keyCode);

		InputManager();
		~InputManager();
	};
}


