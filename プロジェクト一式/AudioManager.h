#pragma once
#include <DirectXMath.h>
using namespace DirectX;

enum AUDIO_SCENE {
	PLAY = 0,
	OTHER,
};

enum AUDIO_ID
{

};

namespace AudioManager {
	void Initialize();
	void Release();
	void SetSceneData(AUDIO_SCENE scene);

	//‰¹‚ğ–Â‚ç‚·
	void Play();

	//‰¹‚ğ–Â‚ç‚·E
	void Play(XMFLOAT3 position, float range);

};