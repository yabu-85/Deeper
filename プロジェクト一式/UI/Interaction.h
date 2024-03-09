#pragma once
#include "../Engine/GameObject.h"

class InteractionUI;
class GameObject;

namespace Interaction {
	void Initialize();
	void Draw();
	void SceneTransitionInitialize();

	//UiList‚Ì‰Šú‰»
	void ResetInteractData();
	//UiList‚É’Ç‰Á
	void AddInteractData(InteractionUI* data);
	//UiList‚©‚çíœ
	void RemoveInteractData(InteractionUI* data);
	
	//ˆø”‚ÌGameObject‚Íˆê”Ô‹ß‚¢UIObject‚È‚Ì‚©
	bool IsMinDistance(GameObject* parent);
	
}

