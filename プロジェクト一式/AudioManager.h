#pragma once
#include <DirectXMath.h>
using namespace DirectX;

enum AUDIO_ID
{

};

namespace AudioManager {
	void Initialize();
	void Release();

	//‰¹‚ğ–Â‚ç‚·
	void Play();

	//‰¹‚ğ–Â‚ç‚·E
	void Play(XMFLOAT3 position, float range);

};