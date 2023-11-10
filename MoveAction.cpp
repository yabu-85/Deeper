#include "MoveAction.h"
#include "Engine/GameObject.h"

MoveAction::MoveAction(std::string name, GameObject* owner)	: Action(name), owner_(owner), speed_(1.0f) {}

MoveAction::Status MoveAction::Update()
{
	XMFLOAT3 pos = owner_->GetPosition();
	XMVECTOR vPos = XMLoadFloat3(&pos);
	XMFLOAT3 targetPos = XMFLOAT3();
	XMVECTOR vTar = XMLoadFloat3(&targetPos);
	XMVECTOR vMove = vTar - vPos;
	vMove = XMVector3Normalize(vMove) * speed_;
	XMStoreFloat3(&pos, vPos + vMove);
	owner_->SetPosition(pos);

	return Status::SUCCESS;
}
