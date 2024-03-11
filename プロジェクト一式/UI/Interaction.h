#pragma once
#include "../Engine/GameObject.h"

class InteractionUI;
class GameObject;

namespace Interaction {
	void Initialize();
	void Draw();
	void SceneTransitionInitialize();

	//UiList�̏�����
	void ResetInteractData();
	//UiList�ɒǉ�
	void AddInteractData(InteractionUI* data);
	//UiList����폜
	void RemoveInteractData(InteractionUI* data);
	
	//������GameObject�͈�ԋ߂�UIObject�Ȃ̂�
	bool IsMinDistance(GameObject* parent);
	
}

