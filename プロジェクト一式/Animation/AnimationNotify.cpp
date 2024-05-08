#include "AnimationNotify.h"
#include "../Other/VFXManager.h"
#include "../Other/AudioManager.h"

void OneFrame::CalcFrame(int t, GameObject* obj) {
	if (time == t) OnFrame(obj); 
}

void DoubleFrame::CalcFrame(int t, GameObject* obj) {
	if (time <= t && endTime >= t) {
		OnFrame(obj);
		if (time == t) OnEnter(obj);
		if (endTime == t) OnExit(obj);
	}
}

void CreatFrame::CalcFrame(int t, GameObject* obj) {
	if (time == t) VFXManager::CreateVfx(XMFLOAT3(), type);
}

void PlaySoundFrame::CalcFrame(int t, GameObject* obj) {
}
