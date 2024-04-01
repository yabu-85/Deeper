#pragma once

namespace Easing
{
	//イージング関数
	float EeaseIn(float i);

	float InQuint(float x);
	float OutQuint(float x);
	float InOutQuint(float x);

	//線形補完
	float Leap(float start, float end, float value);

};
