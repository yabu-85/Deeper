#pragma once
#include <DirectXMath.h>
using namespace DirectX;

enum AUDIO_ID
{

};

namespace AudioManager {
	void Initialize();
	void Release();

	//����炷
	void Play();

	//����炷�E
	void Play(XMFLOAT3 position, float range);

};