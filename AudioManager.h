#pragma once
#include <DirectXMath.h>
using namespace DirectX;

enum AUDIO_ID
{

};

namespace AudioManager {
	void Initialize();
	void Release();

	//音を鳴らす
	void Play();

	//音を鳴らす・
	void Play(XMFLOAT3 position, float range);

};