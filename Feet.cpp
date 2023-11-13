#include "Feet.h"
#include "Engine/Model.h"
#include "NavigationAI.h"
#include "EnemyUi.h"
#include "Engine/SphereCollider.h"
#include "MoveAction.h"
#include "Engine/Global.h"
#include "Player.h"
#include "FeetActionNode.h"
#include "BehaviourNode.h"
#include "TargetConditionCount.h"

Feet::Feet(GameObject* parent)
	:EnemyBase(parent), hModel_(-1), pHandCollider_(nullptr)
{
	objectName_ = "Feet";
}

Feet::~Feet()
{
	Release();
}

void Feet::Initialize()
{
	//モデルデータのロード
	hModel_ = Model::Load("Model/StoneGolem.fbx");
	assert(hModel_ >= 0);
//	Model::SetAnimFrame(hModel_, 0, 100, 1.0f);


	transform_.rotate_.y = -90.0f;
	transform_.scale_ = XMFLOAT3(2.0f, 2.0f, 2.0f);
	transform_.position_ = XMFLOAT3(50.0f + (float)(rand() % 10), 0.0f, 50.0f + (float)(rand() % 10));

	maxHp_ = 10;
	hp_ = maxHp_;
	aimTargetPos_ = 3.0f;
	moveSpeed_ = 0.07f;
	moveRange_ = 5.0f;
	rotateRatio_ = 0.05f;

	SphereCollider* collision1 = new SphereCollider(XMFLOAT3(0, 3, 0), 1.5f);
	SphereCollider* collision2 = new SphereCollider(XMFLOAT3(0, 1, 0), 1.5f);
	pHandCollider_ = new SphereCollider(XMFLOAT3(0, 0, 0), 1.0f);
	AddCollider(collision1);
	AddCollider(collision2);
	AddCollider(pHandCollider_);

	pEnemyUi_ = new EnemyUi(this);
	pEnemyUi_->Initialize(5.5f);

	//ビヘイビアツリーの設定
	root_ = new Root();
	Sequence* seq1 = new Sequence();
	MoveTarget* action1 = new MoveTarget(this, 0.1f, 2.0f);
	FeetJump* action2 = new FeetJump(this);
	
	CombatEnemyCount* condition1 = new CombatEnemyCount(3, action1);
	Inverter* inv1 = new Inverter(condition1);
	FeetCondition1* condition2 = new FeetCondition1(this, action2);
	Succeeder* suc1 = new Succeeder(inv1);

	root_->SetRootNode(seq1);
	seq1->AddChildren(suc1);
	seq1->AddChildren(condition2);
	
}

void Feet::Update()
{
	switch (state_)
	{
	case State::APPEAR:
		break;
	case State::PATROL:
		break;
	case State::COMBAT:
		Player* pPlayer = (Player*)FindObject("Player");
		moveTargetPos_ = pPlayer->GetPosition();
		root_->Update();
		Rotate();
		break;
	case State::DEAD:
		break;
	default:
		break;
	}
	

	if(pEnemyUi_) pEnemyUi_->Update();

}

void Feet::Draw()
{
	XMFLOAT3 center = Model::GetBoneAnimPosition(hModel_, "hand.R");
	center = XMFLOAT3(center.x - transform_.position_.x, center.y - transform_.position_.y, center.z - transform_.position_.z);
	pHandCollider_->SetCenter(center);

	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);

	CollisionDraw();
}

void Feet::Release()
{
	SAFE_DELETE(root_);
}