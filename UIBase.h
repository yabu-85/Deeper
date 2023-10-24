#pragma once
#include <DirectXMath.h>
#include <string>
using namespace DirectX;

class UIBase
{
	int hPict_[2];
	int alpha_;				//�����x
	int frameAlpha_;		//�����x
	float width_;			//�{�^���̕�
	float height_;			//�{�^���̍���
	bool isButtonInactive_; //�{�^�����������m���Ȃ�
	bool isFirstPoint;		//����Ə���
	std::string name_;		//�{�^���ɕ\������Png��
	XMFLOAT3 widePos_;		//-1�`1����Ȃ�,-1280�`1280�Ƃ��̍��W
	XMFLOAT3 frameSize_;	//Frame�̃T�C�Y

public:
	UIBase();


};

