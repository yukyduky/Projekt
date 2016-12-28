#include "Time.h"


Time::Time()
{
	frequency = 0.0;
	CounterStart = 0;
	prevCurrentCount = 0;
}


Time::~Time()
{
}

void Time::StartTimer()
{
	LARGE_INTEGER currentCount;
	// Gets the frequency of the performance counter which is fixed at boot and only needed to be stored once
	QueryPerformanceFrequency(&currentCount);
	frequency = double(currentCount.QuadPart);

	// Get the current value of the performance counter since boot
	QueryPerformanceCounter(&currentCount);
	CounterStart = currentCount.QuadPart;
}

double Time::GetTime()
{
	LARGE_INTEGER currentCount;

	QueryPerformanceCounter(&currentCount);
	return double(currentCount.QuadPart - CounterStart) / frequency;
}

double Time::GetFrameTime()
{
	LARGE_INTEGER currentCount;
	__int64 frameCount = 0.0f;

	QueryPerformanceCounter(&currentCount);

	// Gets the count since last frame
	frameCount = currentCount.QuadPart - prevCurrentCount;
	// Stores the current count for the next frame
	prevCurrentCount = currentCount.QuadPart;

	// Should never happen
	if (frameCount < 0.0f)
		frameCount = 0.0f;

	// Count divided by frequency equals time in milliseconds
	return float(frameCount) / frequency;
}
