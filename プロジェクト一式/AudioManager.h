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

	//����炷
	void Play();

	//����炷�E
	void Play(XMFLOAT3 position, float range);

};