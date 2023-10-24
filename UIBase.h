#pragma once
#include <DirectXMath.h>
#include <string>
#include "Engine/Transform.h"
using namespace DirectX;

//基底クラスだけど、ボタンクラスとしても使う
class UIBase
{
	int hPict_[3];
	int alpha_;
	bool isBound_;			//範囲の内側にいるかどうか
	XMFLOAT2 widePos_;		//0 ~ 1980とかの座標
	XMFLOAT2 frameSize_;	//半径
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

