#pragma once
#include "GlobalDefine.h"
#include "POINT.h"
#include "SingleTone.h"

namespace HEngine
{
	/**
	* @brief 엔진의 Input을 담당하는 클레스
	* @details 키입력을 담당하는 클레스입니다. RegistKeyCode로 등록후 isKeyPress , isKeyUp , isKeyDown 함수로 상태를 검출합니다.
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


