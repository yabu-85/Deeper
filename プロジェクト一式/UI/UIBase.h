#pragma once
#include <DirectXMath.h>
#include <string>
#include <functional>
#include "../Engine/Transform.h"

using namespace DirectX;

//UI（今はボタンクラスになってる
class UIBase
{
	int hPict_[3];
	int alpha_;
	bool isBound_;			//範囲の内側にいるかどうか
	XMFLOAT2 widePos_;		//0 ~ 1980とかの座標
	XMFLOAT2 frameSize_;	//半径
	Transform transform_;

	enum BUTTON_STATE {
		PRESSED = 0,
		UPPRESSED,
	};

	std::function<void()> onClick_;

public:
	UIBase();
	~UIBase();
	void Initialize(std::string name, XMFLOAT2 pos, XMFLOAT2 size, std::function<void()> onClick);
	void Draw();
	bool IsWithinBound();
	void OnClick();

};

