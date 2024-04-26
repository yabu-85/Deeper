#pragma once
#include <DirectXMath.h>
using namespace DirectX;

enum class AUDIO_ID {
	BUTTON_WITHIN = 0,
	BUTTON_PUSH,
	PAUSE_OPEN,
	PAUSE_CLOSE,
	SWORD_WIELD_1,
	SWORD_WIELD_2,
	SWORD_WIELD_3,
	BULLET_HIT1,
};

namespace AudioManager {
	void Initialize();
	void Release();
	void SetAudioData();

	//����炷
	void Play(AUDIO_ID id, float volume = 1.0f);
	
	/// <summary>
	/// ����炷�E�����ɂ���ĉ��ʂ�ς��ĉ��ʂ�ς���
	/// range�͈͓̔���Combat��Ԃ���Ȃ��G�l�~�[��Combat��Ԃ�
	/// </summary>
	void Play(AUDIO_ID id, XMFLOAT3 position, float range, float volume = 1.0f);

};