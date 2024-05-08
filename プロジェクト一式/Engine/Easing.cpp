#include "Easing.h"
#include <cmath>

float Easing::QuintIn(float x)
{
	return x * x * x * x * x;
}

float Easing::QuintOut(float x)
{
	return 1.0f - pow(1.0f - x, 5.0f);
}

float Easing::QuintInOut(float x)
{
	return (x < 0.5f) ? 16.0f * x * x * x * x * x : 1.0f - pow(-2.0f * x + 2.0f, 5.0f) / 2.0f;
}

float Easing::ExpoIn(float x)
{
	return x == 0.0f ? 0.0f : pow(2.0f, 10.0f * x - 10.0f);
}

float Easing::Leap(float start, float end, float value)
{
	return start + value * (end - start);
}