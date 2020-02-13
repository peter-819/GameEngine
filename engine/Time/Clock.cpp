#include "Clock.h"
namespace Time {

	bool Clock::clockInitialize() {
		bool errorCode = QueryPerformanceFrequency(&nowPerformanceFrequency);
		deltaTime = 0;
		lastFrameCounter.QuadPart = 0;
		return errorCode;
	}

	bool Clock::clockGetNewFrame() {
		LARGE_INTEGER nowFrameCounter;
		bool errorCode = QueryPerformanceCounter(&nowFrameCounter);
		if (!errorCode) return false;
		deltaCounter.QuadPart = nowFrameCounter.QuadPart - lastFrameCounter.QuadPart;
		if (lastFrameCounter.QuadPart != 0) 
			deltaTime = ((float)deltaCounter.QuadPart) / nowPerformanceFrequency.QuadPart;
		lastFrameCounter.QuadPart = nowFrameCounter.QuadPart;
		return true;
	}

	float Clock::clockTimeLastFrame() {
		return deltaTime;
	}

	bool Clock::clockShutdown() {
		return true;
	}
}