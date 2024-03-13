#pragma once
#include <DirectXMath.h>
#include <string>
#include <functional>
#include "../Engine/Transform.h"

using namespace DirectX;

class UIBase
{
	int hPict_[3];
	int alpha_;
	bool isBound_;			//îÕàÕÇÃì‡ë§Ç…Ç¢ÇÈÇ©Ç«Ç§Ç©
	XMFLOAT2 widePos_;		//0 ~ 1980Ç∆Ç©ÇÃç¿ïW
	XMFLOAT2 frameSize_;	//îºåa
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

