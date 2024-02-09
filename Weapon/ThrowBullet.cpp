#include "ThrowBullet.h"
#include "../Engine/Model.h"
#include "../Engine/SphereCollider.h"
#include "../GameManager.h"
#include "../AudioManager.h"
#include "../VFXManager.h"
#include "../Enemy/EnemyBase.h"

ThrowBullet::ThrowBullet(GameObject* parent)
	: BulletBase(parent, "ThrowBullet"), damage_(0)
{
}

ThrowBullet::~ThrowBullet()
{
	Release();
}

void ThrowBullet::Initialize()
{
	//モデルデータのロード
	hModel_ = Model::Load("DebugCollision/SphereCollider.fbx");
	assert(hModel_ >= 0);

	transform_.scale_ = XMFLOAT3(0.1f, 0.1f, 0.1f);
	velocity_ = 0.7f;
	lifeTime_ = 30;
	damage_ = rand() % 11;

	SphereCollider* collision = new SphereCollider(XMFLOAT3(0, 0, 0), 0.5f);
	AddAttackCollider(collision);

}

void ThrowBullet::Update()
{
	LifeTime();
	Move();

	//CollisionMapとの判定（今はy<=0だけ）
	if (transform_.position_.y <= 0.0f) {
		VFXManager::CreatVfxExplode1(transform_.position_);
		AudioManager::Play(transform_.position_, 10.0f);
		KillMe();
	}

}

void ThrowBullet::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void ThrowBullet::Release()
{
	Model::Release(hModel_);
}

void ThrowBullet::OnAttackCollision(GameObject* pTarget)
{
	if (pTarget->GetObjectName() == "Player") {
		VFXManager::CreatVfxExplode1(transform_.position_);
		AudioManager::Play(transform_.position_, 10.0f);
		KillMe();

	}

}

//moveVec_をTargetの位置として使う
void ThrowBullet::Shot(XMFLOAT3 pos, XMFLOAT3 target)
{
	transform_.position_ = pos;
	moveVec_ = target;

}

void ThrowBullet::Move()
{
	const float damping = 0.9f;				//空気抵抗
	const float propulsion = 0.9f;			//推進体
	const float speed = 0.1f;				//移動スピード
	const float curvatureRadius = 0.5f;		//半径何メートルの円で曲がれるのかみたいな
	const float maxCentripetalAccel = speed * speed / curvatureRadius;	//最大向心加速度
	
	XMVECTOR toTarget = XMLoadFloat3(&moveVec_) - XMLoadFloat3(&transform_.position_);
	XMVECTOR vn = XMVector3Normalize(vVelocity_);

	XMFLOAT3 vec1, vec2;
	XMStoreFloat3(&vec1, toTarget);
	XMStoreFloat3(&vec2, vn); 
	float dot = vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
	
	XMVECTOR centripetalAccel = toTarget - (vn * dot);
	float centripetalAccelMagnitude = XMVectorGetX(XMVector3Length(centripetalAccel));
	if (centripetalAccelMagnitude > 1.0f)
	{
		centripetalAccel /= centripetalAccelMagnitude;
	}

	XMVECTOR force = centripetalAccel * maxCentripetalAccel;
	force += vn * propulsion;
	force -= vVelocity_ * damping;
	vVelocity_ += force;
	force = XMLoadFloat3(&transform_.position_) + vVelocity_;
	XMStoreFloat3(&transform_.position_, force);

}
