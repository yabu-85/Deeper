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
	:EnemyBase(parent), hModel_(-1)
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

	transform_.rotate_.y = -90.0f;
	transform_.scale_ = XMFLOAT3(2.0f, 2.0f, 2.0f);
	transform_.position_ = XMFLOAT3(50.0f + (float)(rand() % 10), 0.0f, 50.0f + (float)(rand() % 10));

	maxHp_ = 10;
	hp_ = maxHp_;
	aimTargetPos_ = 3.0f;
	moveSpeed_ = 0.07f;
	moveRange_ = 5.0f;
	rotateRatio_ = 0.1f;

	SphereCollider* collision1 = new SphereCollider(XMFLOAT3(0, 3, 0), 1.5f);
	SphereCollider* collision2 = new SphereCollider(XMFLOAT3(0, 1, 0), 1.5f);
	AddCollider(collision1);
	AddCollider(collision2);

	pEnemyUi_ = new EnemyUi(this);
	pEnemyUi_->Initialize(5.5f);

	//ビヘイビアツリーの設定
	root_ = new Root();
	Sequence* seq1 = new Sequence();
	MoveTarget* action1 = new MoveTarget(this, 0.1f, 2.0f);
	FeetJump* action2 = new FeetJump(this);
	
	TargetConditionCount* condition1 = new TargetConditionCount(5, action1);
	FeetCondition1* condition2 = new FeetCondition1(this, action2);
	Succeeder* suc1 = new Succeeder(condition1);

	root_->SetRootNode(seq1);

	seq1->AddChildren(suc1);
	seq1->AddChildren(condition2);
	
}

void Feet::Update()
{
	Player* pPlayer = (Player*)FindObject("Player");
	moveTargetPos_ = pPlayer->GetPosition();

	state_ = State::IDLE;
	root_->Update();
	pEnemyUi_->Update();
	Rotate();
}

void Feet::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);

	CollisionDraw();
}

void Feet::Release()
{
	SAFE_DELETE(root_);
}