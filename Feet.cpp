#include "Feet.h"
#include "Engine/Model.h"
#include "NavigationAI.h"
#include "EnemyUi.h"
#include "Engine/SphereCollider.h"
#include "FeetActionNode.h"

namespace {	
	float moveSpeed = 0.2f;

}

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
	hModel_ = Model::Load("Model/Feet.fbx");
	assert(hModel_ >= 0);

	transform_.rotate_.y = -90.0f;
	transform_.scale_ = XMFLOAT3(0.3f, 0.3f, 0.3f);
	transform_.position_ = XMFLOAT3(50.0f + (float)(rand() % 10), 0.0f, 50.0f + (float)(rand() % 10));

	maxHp_ = 10;
	hp_ = maxHp_;

	SphereCollider* collision = new SphereCollider(XMFLOAT3(0, 1, 0), 1.9f);
	AddCollider(collision);

	pEnemyUi_ = new EnemyUi(this);
	pEnemyUi_->Initialize();

	seq1_ = new BT::Sequence("seq1");
	BT::CompositeNode *con1 = new BT::CompositeNode("com1");
	FeetMove* action1 = new FeetMove("act1");

	seq1_->AddChildren(con1);


}

void Feet::Update()
{
	pEnemyUi_->Update();

	seq1_->Tick();

}

void Feet::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);

	CollisionDraw();

}

void Feet::Release()
{
}

void Feet::ApplyDamage(int da)
{


}

/*
void FeetWalk::Update()
{
	XMFLOAT3 pos = pFeet_->GetPosition();
	XMVECTOR vPos = XMLoadFloat3(&pos);
	XMVECTOR vTar = XMLoadFloat3(&targetPos_);
	XMVECTOR vMove = vTar - vPos;
	vMove = XMVector3Normalize(vMove) * moveSpeed;
	XMStoreFloat3(&pos, vPos + vMove);
	pFeet_->SetPosition(pos);

	float length = XMVectorGetX(XMVector3Length(vTar - vPos));
	if (length <= moveSpeed) owner_->ChangeState("Wait");
}*/
