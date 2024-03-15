#pragma once
#include "../Engine/GameObject.h"

class InteractionUI;
class GameObject;

namespace Interaction {
	void Initialize();
	void Draw();
	void SceneTransitionInitialize();

	//UiListの初期化
	void ResetInteractData();
	//UiListに追加
	void AddInteractData(InteractionUI* data);
	//UiListから削除
	void RemoveInteractData(InteractionUI* data);
	
	//引数のGameObjectは一番近いUIObjectなのか
	bool IsMinDistance(GameObject* parent);
	
	//Interactオブジェクトの取得率
	void SetParcent(float f);
}

