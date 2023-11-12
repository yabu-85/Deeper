#include "EnemyBase.h"
#include "EnemyUi.h"
#include "GameManager.h"
#include "DropTable.h"
#include "EnemySpawnCtrl.h"
#include "Player.h"

EnemyBase::EnemyBase(GameObject* parent)
	: GameObject(parent), pEnemyUi_(nullptr), hp_(0), maxHp_(0), type_(ENEMY_MAX), moveTargetPos_(0.0f, 0.0f, 0.0f), state_(State::IDLE),
	moveSpeed_(0.1f), moveRange_(1.0f), prePos_(0.0f, 0.0f, 0.0f), rotateRatio_(1.0f), aimTargetPos_(0.0f)
{
}

bool EnemyBase::Move()
{
	XMFLOAT3 pos = transform_.position_;
	XMVECTOR vPos = XMLoadFloat3(&pos);
	XMVECTOR vTar = XMLoadFloat3(&moveTargetPos_);
	XMVECTOR vMove = vTar - vPos;
	vMove = XMVector3Normalize(vMove) * moveSpeed_;

	//TargetˆÊ’u‚Â‚¢‚½
	float length = XMVectorGetX(XMVector3Length(vTar - vPos));
	if (length <= moveRange_) {
		return false;
	}

	prePos_ = transform_.position_;
	XMStoreFloat3(&transform_.position_, vPos + vMove);
	
	return true;
}

void EnemyBase::Rotate()
{
	Player* pPlayer = (Player*)FindObject("Player");
	XMFLOAT3 plaPos = pPlayer->GetPosition();

    XMFLOAT2 a = XMFLOAT2(sinf(XMConvertToRadians(transform_.rotate_.y)), cosf(XMConvertToRadians(transform_.rotate_.y)));
    XMVECTOR vA = XMVector2Normalize(XMLoadFloat2(&a));
	XMFLOAT2 b(transform_.position_.x - plaPos.x, transform_.position_.z - plaPos.z);
	XMVECTOR vB = XMVector2Normalize(XMLoadFloat2(&b)) * -1.0f;
	XMStoreFloat2(&a, vA);
    XMStoreFloat2(&b, vB);
    float cross = a.x * b.y - a.y * b.x;
    float dot = a.x * b.x + a.y * b.y;
    transform_.rotate_.y += XMConvertToDegrees(-atan2f(cross, dot) * rotateRatio_);
}

void EnemyBase::ApplyDamage(int da)
{
	hp_ -= da;
	pEnemyUi_->SetParcent((float)(hp_) / (float)(maxHp_));
	if (hp_ <= 0) {
		GameManager::GetDropTable()->DropItem(0, transform_.position_);
		GameManager::GetEnemySpawnCtrl()->KillEnemy(this);
	}
}
