#ifndef ENGINE_TIME_CLOCK_H
#define ENGINE_TIME_CLOCK_H

#include <Windows.h>
namespace Time {
	class __declspec(dllexport) Clock {			//����lib�ļ�
	private:
		LARGE_INTEGER lastFrameCounter;			//��һ֡������
		LARGE_INTEGER deltaCounter;				//��ֵ������
		float deltaTime;						//��ֵʱ��
	public:
		LARGE_INTEGER nowPerformanceFrequency;	//ÿ��������
		bool clockInitialize();					//clock��ʼ��
		bool clockGetNewFrame();				//��ȡ��һ֡
		float clockTimeLastFrame();				//��ȡ֡��ֵʱ��
		bool clockShutdown();					//clock�ر�
	};
}

#endif