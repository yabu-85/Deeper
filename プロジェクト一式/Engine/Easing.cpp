#include "Easing.h"
#include <cmath>

float Easing::InQuint(float x)
{
	return x * x * x * x * x;
}

float Easing::OutQuint(float x)
{
	return 1.0f - pow(1.0f - x, 5.0f);
}

float Easing::InOutQuint(float x)
{
	return (x < 0.5f) ? 16.0f * x * x * x * x * x : 1.0f - pow(-2.0f * x + 2.0f, 5.0f) / 2.0f;
}