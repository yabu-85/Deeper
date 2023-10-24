#pragma once
#include <DirectXMath.h>
#include <string>
using namespace DirectX;

class UIBase
{
	int hPict_[2];
	int alpha_;				//透明度
	int frameAlpha_;		//透明度
	float width_;			//ボタンの幅
	float height_;			//ボタンの高さ
	bool isButtonInactive_; //ボタンを押す感知しない
	bool isFirstPoint;		//初回照準か
	std::string name_;		//ボタンに表示するPngの
	XMFLOAT3 widePos_;		//-1～1じゃなく,-1280～1280とかの座標
	XMFLOAT3 frameSize_;	//Frameのサイズ

public:
	UIBase();


};

