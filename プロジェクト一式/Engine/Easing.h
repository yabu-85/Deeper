#pragma once

namespace Easing
{
	//イージング関数
	float EeaseIn(float i);

	float QuintIn(float x);
	float QuintOut(float x);

	float QuintInOut(float x);
	
	float ExpoIn(float x);

	//線形補完
	float Leap(float start, float end, float value);

};
