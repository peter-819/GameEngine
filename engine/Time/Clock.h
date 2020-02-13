#ifndef ENGINE_TIME_CLOCK_H
#define ENGINE_TIME_CLOCK_H

#include <Windows.h>
namespace Time {
	class __declspec(dllexport) Clock {			//生成lib文件
	private:
		LARGE_INTEGER lastFrameCounter;			//上一帧周期数
		LARGE_INTEGER deltaCounter;				//差值周期数
		float deltaTime;						//差值时间
	public:
		LARGE_INTEGER nowPerformanceFrequency;	//每秒周期数
		bool clockInitialize();					//clock初始化
		bool clockGetNewFrame();				//获取新一帧
		float clockTimeLastFrame();				//获取帧差值时间
		bool clockShutdown();					//clock关闭
	};
}

#endif