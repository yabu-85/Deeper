#pragma once
#include "../Engine/GameObject.h"

class InteractionUI;
class GameObject;

namespace Interaction {
	void Initialize();
	void Draw();
	void SceneTransitionInitialize();

	void ResetInteractData();
	void AddInteractData(InteractionUI* data);
	void RemoveInteractData(InteractionUI* data);
	
	bool IsMinDistance(GameObject* parent);
	
}

