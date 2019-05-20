#pragma once
#include <Windows.h>
template <typename T>
class SingleTone
{
private:
	static T* m_pThis;
protected:
	SingleTone()
	{

	}
	virtual ~SingleTone()
	{

	}

public:
	static T * GetInstance()
	{
		if (m_pThis == NULL)
			m_pThis = new T();
		return m_pThis;
	};

	static void DestroyInstance()
	{
		if (m_pThis)
		{
			delete m_pThis;
			m_pThis = NULL;
		}
	}
};

template <typename T> T * SingleTone<T>::m_pThis = 0;
