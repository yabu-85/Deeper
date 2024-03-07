#pragma once
#include <DirectXMath.h>
using namespace DirectX;
class GameObject;

class InteractionUI
{
	bool isValid_;
	XMFLOAT3 offset_;
	GameObject* pParent_;

public:
	InteractionUI(GameObject* parent);
	~InteractionUI();
	void Initialize();
	void Release();

	void SetValid(bool b) { isValid_ = b; }
	bool GetValid() { return isValid_; }
	void SetOffset(XMFLOAT3 pos) { offset_ = pos; }
	XMFLOAT3 GetOffset() { return offset_; }
	GameObject* GetParent() { return pParent_; }

};

