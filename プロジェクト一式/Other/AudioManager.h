#pragma once
#include <DirectXMath.h>
using namespace DirectX;

enum AUDIO_TYPE {
	BUTTON_WITHIN = 0,
	BUTTON_PUSH,
	PAUSE_OPEN,
	PAUSE_CLOSE,
	SWORD_WIELD_1,
	SWORD_WIELD_2,
	SWORD_WIELD_3,
	BULLET_HIT1,
};

//音を鳴らす機能をまとめた
namespace AudioManager {
	void Initialize();
	void Release();
	void SetAudioData();

	//音を鳴らす
	void Play(AUDIO_TYPE id, float volume = 1.0f);
	
	/// <summary>
	/// 音を鳴らす・距離によって音量を変えて音量を変える
	/// rangeの範囲内のCombat状態じゃないエネミーをCombat状態に
	/// </summary>
	void Play(AUDIO_TYPE id, XMFLOAT3 position, float range, float volume = 1.0f);

};