#include "MasterHand.h"
#include "Engine/Model.h"
#include "NavigationAI.h"
#include "GameManager.h"

#include "Aim.h"
#include "Player.h"

namespace {
	const int killTime = 60;
	const float moveSpeed = 5.0f;

}

MasterHand::MasterHand(GameObject* parent)
	: EnemyBase(parent), hModel_(-1), lifeTime_(0), pNavigationAI_(nullptr)
{
	objectName_ = "MasterHand";
}

MasterHand::~MasterHand()
{
}

void MasterHand::Initialize()
{
	//モデルデータのロード
	hModel_ = Model::Load("Model/Hand.fbx");
	assert(hModel_ >= 0);

	transform_.rotate_.y = -90;

	GameManager* pGameManager = (GameManager*)FindObject("GameManager");
	pNavigationAI_ = pGameManager->GetNavigationAI();

	//ますたーはんど発射ようの
	Aim* pAim = (Aim*)FindObject("Aim");
	Player* pPlayer = (Player*)FindObject("Player");
	transform_.rotate_.y = pAim->GetRotate().y;
	transform_.rotate_.y -= 180.0f;
	transform_.position_ = pPlayer->GetPosition();

}

void MasterHand::Update()
{
	if (lifeTime_ >= killTime) KillMe();
	lifeTime_++;

	//向いている方向へ前進
	XMVECTOR vPos = XMLoadFloat3(&transform_.position_);
	XMVECTOR vMove = { 0.0f, 0.0f, 0.1f, 0.0f };
	CXMMATRIX mRotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y + 180.0f));
	vMove = XMVector3TransformCoord(vMove, mRotY) * moveSpeed;
	vPos -= vMove;
	XMStoreFloat3(&transform_.position_, vPos);

}

void MasterHand::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);

}

void MasterHand::Release()
{
}
