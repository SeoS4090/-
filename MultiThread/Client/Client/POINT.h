#pragma once
#include <Windows.h>

namespace HEngine
{
	/**
	* @brief 2���� int ��ǥ�� ǥ���ϴ� Ŭ����
	* @details 2���� int ��ǥ�� ǥ���ϴ� Ŭ����
	* @author JBC classicaa@naver.com
	* @date 2016-10-14
	* @version 0.0.1
	*/

	class POINT
	{
	public:
		int x;
		int y;

		POINT();
		POINT(int x_, int y_);
		~POINT();
	};

	/**
	* @brief 2���� float ��ǥ�� ǥ���ϴ� Ŭ����
	* @details 2���� float ��ǥ�� ǥ���ϴ� Ŭ����
	* @author JBC classicaa@naver.com
	* @date 2016-10-14
	* @version 0.0.1
	*/

	class POINTF
	{
	public:
		float x;
		float y;

		POINTF();
		POINTF(float x_, float y_);
		~POINTF();
	};

	/**
	* @brief RECT ��ǥ�� ǥ���ϴ� Ŭ����
	* @details RECT ��ǥ�� ǥ���ϴ� Ŭ����
	* @author JBC classicaa@naver.com
	* @date 2016-10-14
	* @version 0.0.1
	*/

	class RECT
	{
	public:
		int left;
		int top;
		int right;
		int bottom;

		inline void Set(int left_, int top_, int right_, int bottom_)
		{
			left = left_;
			top = top_;
			right = right_;
			bottom = bottom_;
		}

		inline ::RECT ToWinRECT()
		{
			::RECT rc = { left , top , right , bottom };
			return rc;
		}

		bool isCollision(RECT rcOther);
		bool isPtin(POINT ptOther);
		RECT();
		RECT(int left_, int top_, int right_, int bottom_);
		~RECT();
	};
}

