#include "Easing.h"
#include <cmath>

#include "Direct3D.h"
#include "GameObject.h"

float Easing::EaseInQuint(float x)
{
	return x * x * x * x * x;
}

float Easing::EaseOutQuint(float x)
{
	return 1.0f - pow(1.0f - x, 5.0f);
}

float Easing::EaseInOutQuint(float x)
{
	return x < 0.5f ? (16.0f * x * x * x * x * x) : (1.0f - pow(-2.0f * x + 2.0f, 5.0f) / 2.0f);
}


float Easing::Leap(float start, float end, float x)
{
	return start + x * (end - start);
}

//階乗計算を行う関数
double Factorial(int a)
{
    double result = 1.0f;
    for (int i = 2; i <= a; i++)
    {
        result *= i;
    }

    return result;
}

//二項係数を計算する関数
double Binomial(int n, int k)
{
    return Factorial(n) / (Factorial(k) * Factorial(n - k));
}

//バーンスタイン基底関数
double Bernstein(int n, int i, float t)
{
    return Binomial(n, i) * pow(t, i) * pow(1 - t, n - i);
}