#pragma once
#ifndef TIME_H
#define TIME_H

#include <dwrite.h>

class Time
{
public:
	Time();
	~Time();

	// Starts the timer
	void StartTimer();
	// Returns the time that the application has been running for
	double GetTime();
	// Returns the time since the last frame
	double GetFrameTime();

private:
	// Variables
	double frequency;
	__int64 CounterStart;
	__int64 prevCurrentCount;
};

#endif
