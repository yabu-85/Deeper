#pragma once

namespace Easing
{
	//�C�[�W���O�֐�
	float EeaseIn(float i);

	float InQuint(float x);
	float OutQuint(float x);
	float InOutQuint(float x);

	//���`�⊮
	float Leap(float start, float end, float value);

};
