#pragma once
#include <DirectXMath.h>
#include <string>
#include "Engine/Transform.h"
using namespace DirectX;

//���N���X�����ǁA�{�^���N���X�Ƃ��Ă��g��
class UIBase
{
	int hPict_[3];
	int alpha_;
	bool isBound_;			//�͈͂̓����ɂ��邩�ǂ���
	XMFLOAT2 widePos_;		//0 ~ 1980�Ƃ��̍��W
	XMFLOAT2 frameSize_;	//���a
	std::string name_;
	Transform transform_;

	enum BUTTON_STATE {
		PRESSED = 0,
		UPPRESSED,
	};

	bool IsWithinBound();

public:
	UIBase();
	void Initialize(std::string name);
	void Update();
	void Draw();

};

