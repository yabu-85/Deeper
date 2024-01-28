#include "MasterHand.h"
#include "Engine/Model.h"
#include "GameManager.h"
#include "EnemyUi.h"
#include "DropTable.h"
#include "Engine/Global.h"
#include "Engine/SphereCollider.h"
#include "Aim.h"
#include "Player.h"

namespace {
	const int killTime = 60;
	const float moveSpeed = 5.0f;
}

MasterHand::MasterHand(GameObject* parent)
	: EnemyBase(parent, "MasterHandEnemy"), hModel_(-1), lifeTime_(0)
{
}

MasterHand::~MasterHand()
{

}

void MasterHand::Initialize()
{
	//モデルデータのロード
	hModel_ = Model::Load("Model/Hand.fbx");
	assert(hModel_ >= 0);

	maxHp_ = 10;
	hp_ = maxHp_;

	//ますたーはんど発射ようの
	Aim* pAim = static_cast<Aim*>(FindObject("Aim"));
	Player* pPlayer = static_cast<Player*>(FindObject("Player"));

	transform_.position_ = pPlayer->GetPosition();
	transform_.rotate_.y = pAim->GetRotate().y;
	transform_.rotate_.y -= 180.0f;
	
	//Colliderの設定
	SphereCollider* collision1 = new SphereCollider(XMFLOAT3(0, 1, 0), 1.5f);
	AddCollider(collision1);

	pEnemyUi_ = new EnemyUi(this);
	pEnemyUi_->Initialize(3.0f);

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

	CollisionDraw();

}

void MasterHand::Release()
{
	SAFE_DELETE(pEnemyUi_);

}