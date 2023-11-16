#include "ActionMove.h"
#include "Player.h"

ActionMove::ActionMove(GameObject* obj) : BaseAction(obj), pPlayer_(nullptr), isInRange_(false), moveSpeed_(0.1f), moveRange_(1.0f)
{
}

void ActionMove::Update()
{
	XMFLOAT3 pos = pGameObject_->GetPosition();
	XMVECTOR vPos = XMLoadFloat3(&pos);
	XMFLOAT3 plaPos = pPlayer_->GetPosition();
	XMVECTOR vTar = XMLoadFloat3(&plaPos);
	XMVECTOR vMove = vTar - vPos;
	vMove = XMVector3Normalize(vMove) * moveSpeed_;

	//Target�ʒu����
	float length = XMVectorGetX(XMVector3Length(vTar - vPos));
	if (length <= moveRange_) {
		isInRange_ = true;
		return;
	}

	XMStoreFloat3(&pos, vPos + vMove);
	pGameObject_->SetPosition(pos);
	isInRange_ = false;;
}

void ActionMove::Initialize()
{
	pPlayer_ = static_cast<Player*>(pGameObject_->FindObject("Player"));
}

void ActionMove::Terminate()
{
}
