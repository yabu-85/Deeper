#include "Feet.h"
#include "Engine/Model.h"
#include "NavigationAI.h"
#include "GameManager.h"
#include "EnemyUi.h"
#include "DropTable.h"
#include "Engine/SphereCollider.h"
#include "StateManager.h"

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
	
	pStateManager_ = new StateManager(this);
	pStateManager_->AddState(new FeetWait(pStateManager_));
	pStateManager_->ChangeState("Wait");
	pStateManager_->Initialize();

	pEnemyUi_ = new EnemyUi(this);
	pEnemyUi_->Initialize();

	maxHp_ = 10;
	hp_ = maxHp_;

	SphereCollider* collision = new SphereCollider(XMFLOAT3(0, 1, 0), 1.9f);
	AddCollider(collision);
}

void Feet::Update()
{
	pEnemyUi_->Update();

	return;
	pStateManager_->Update();

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
	hp_ -= da;
	pEnemyUi_->SetParcent((float)(hp_) / (float)(maxHp_));
	if (hp_ <= 0) {
		GameManager* gm = (GameManager*)FindObject("GameManager");
		gm->GetDropTable()->DropItem(0, transform_.position_);
		KillMe();
	}

}

//---------------------------------state-------------------------------

FeetWait::FeetWait(StateManager* owner)
	:time_(0)
{
	owner_ = owner;
	pFeet_ = static_cast<Feet*>(owner_->GetGameObject());
}

void FeetWait::Update()
{
	time_--;
	if (time_ <= 0) owner_->ChangeState("Walk");
}

void FeetWait::OnEnter()
{
	time_ = 60;
}

//---------------------------------------------------------------------

FeetWalk::FeetWalk(StateManager* owner)
	:targetPos_(0.0f,0.0f,0.0f)
{
	owner_ = owner;
	pFeet_ = static_cast<Feet*>(owner_->GetGameObject());
}

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
}

void FeetWalk::OnEnter()
{
	if (rand() % 2 == 0) return; //てきとうらんど

	GameManager* pGameManager = (GameManager*)pFeet_->FindObject("GameManager");
	NavigationAI* pNavigationAI = pGameManager->GetNavigationAI();
	pNavigationAI->Navi(targetPos_);
}

//---------------------------------------------------------------------

FeetDead::FeetDead(StateManager* owner)
{
	owner_ = owner;
	pFeet_ = static_cast<Feet*>(owner_->GetGameObject());
}

void FeetDead::Update()
{
}

void FeetDead::OnEnter()
{
}
