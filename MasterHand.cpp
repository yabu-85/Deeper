#include "MasterHand.h"
#include "Engine/Model.h"
#include "GameManager.h"
#include "EnemyUi.h"
#include "DropTable.h"

#include "Aim.h"
#include "Player.h"

namespace {
	const int killTime = 60;
	const float moveSpeed = 5.0f;
}

MasterHand::MasterHand(GameObject* parent)
	: EnemyBase(parent), hModel_(-1), lifeTime_(0)
{
	objectName_ = "MasterHand";
}

MasterHand::~MasterHand()
{
	Release();

}

void MasterHand::Initialize()
{
	//モデルデータのロード
	hModel_ = Model::Load("Model/Hand.fbx");
	assert(hModel_ >= 0);

	transform_.rotate_.y = -90;

	//ますたーはんど発射ようの
	Aim* pAim = (Aim*)FindObject("Aim");
	Player* pPlayer = (Player*)FindObject("Player");
	transform_.rotate_.y = pAim->GetRotate().y;
	transform_.rotate_.y -= 180.0f;
	transform_.position_ = pPlayer->GetPosition();

	pEnemyUi_ = new EnemyUi(this);
	pEnemyUi_->Initialize(3.0f);

	maxHp_ = 10;
	hp_ = maxHp_;
}

void MasterHand::Update()
{
	if (lifeTime_ >= killTime) {
		GameManager::GetDropTable()->DropItem(0, transform_.position_);
		GameManager::GetEnemyManager()->KillEnemy(this);
		return;
	}
	lifeTime_++;

	//向いている方向へ前進
	XMVECTOR vPos = XMLoadFloat3(&transform_.position_);
	XMVECTOR vMove = { 0.0f, 0.0f, 0.1f, 0.0f };
	XMMATRIX mRotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y + 180.0f));
	vMove = XMVector3TransformCoord(vMove, mRotY) * moveSpeed;
	vPos -= vMove;
	XMStoreFloat3(&transform_.position_, vPos);

}

void MasterHand::Draw()
{
	if(pEnemyUi_) pEnemyUi_->Draw();

	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);

}

void MasterHand::Release()
{
}