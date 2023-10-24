#pragma once
#include <DirectXMath.h>
#include <string>
#include "Engine/Transform.h"
using namespace DirectX;

class UIBase
{
	int hPict_[3];
	int alpha_;
	bool isBound_;			//”ÍˆÍ‚Ì“à‘¤‚É‚¢‚é‚©‚Ç‚¤‚©
	std::string name_;
	Transform transform_;

	enum BUTTON_STATE {
		PRESSED = 0,
		UPPRESSED,
	};

public:
	UIBase();
	void Initialize(std::string name);
	void Draw();

};

