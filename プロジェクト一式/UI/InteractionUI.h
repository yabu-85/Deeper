#pragma once
#include <DirectXMath.h>
using namespace DirectX;
class GameObject;

class InteractionUI
{
	bool isValid_;			//���s�\��
	bool longPress_;		//���������ǂ���

	XMFLOAT3 offset_;		//�\���ʒu
	GameObject* pParent_;	//�e

public:
	InteractionUI(GameObject* parent);
	~InteractionUI();
	void Initialize();
	void Release();

	void SetValid(bool b) { isValid_ = b; }
	bool GetValid() { return isValid_; }
	void SetLongPress(bool b) { longPress_ = b; }
	bool GetLongPress() { return longPress_; }

	void SetOffset(XMFLOAT3 pos) { offset_ = pos; }
	XMFLOAT3 GetOffset() { return offset_; }
	GameObject* GetParent() { return pParent_; }

};

