#pragma once
#include "../Engine/GameObject.h"

namespace Interaction {
	void Initialize();
	void Update();
	void Draw();

	void SetInteract(XMFLOAT3 pos);
	void SetInteract(XMFLOAT3 pos, float length);
	bool IsWarp();

}

