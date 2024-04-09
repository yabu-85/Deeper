#pragma once
#include <DirectXMath.h>
using namespace DirectX;
class GameObject;

class InteractionUI
{
	bool isValid_;			//実行可能か
	bool longPress_;		//長押しかどうか

	XMFLOAT3 offset_;		//表示位置
	GameObject* pParent_;	//親

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

