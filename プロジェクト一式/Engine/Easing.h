#pragma once

namespace Easing
{
	//�C�[�W���O�֐�
	float EeaseIn(float i);

	float QuintIn(float x);
	float QuintOut(float x);

	float QuintInOut(float x);
	
	float ExpoIn(float x);

	//���`�⊮
	float Leap(float start, float end, float value);

};
