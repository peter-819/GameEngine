#include <gtest/gtest.h>
#include <Time/Clock.h>

using Time::Clock;

TEST(TimeTest, ClockInitialize) {
	Clock clock;
	int errorCode = clock.clockInitialize();
	EXPECT_TRUE(errorCode);
}

TEST(TimeTest, ClockGetNewFrame) {
	Clock clock;
	clock.clockInitialize();
	LARGE_INTEGER nowP = clock.nowPerformanceFrequency;
	LARGE_INTEGER beginCounter, endCounter, deltaCounter;

	clock.clockGetNewFrame();
	QueryPerformanceCounter(&beginCounter);
	Sleep(1000);
	clock.clockGetNewFrame();
	QueryPerformanceCounter(&endCounter);

	deltaCounter.QuadPart = endCounter.QuadPart - beginCounter.QuadPart;
	float deltaTime = (float)deltaCounter.QuadPart / nowP.QuadPart;

	EXPECT_NEAR(deltaTime, 1.0f, 0.1f);

	float lastFrameTime = clock.clockTimeLastFrame();
	EXPECT_NEAR(deltaTime, lastFrameTime, 0.1f);
}