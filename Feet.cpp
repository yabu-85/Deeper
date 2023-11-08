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
	:EnemyBase(parent), hModel_(-1), pNavigationAI_(nullptr), targetPos_(0.0f, 0.0f, 0.0f), currtime_(0), stopTime_(60), flag_(true)
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
	targetPos_ = transform_.position_;

	GameManager* pGameManager = (GameManager*)FindObject("GameManager");
	pNavigationAI_ = pGameManager->GetNavigationAI();
	
	pStateManager_ = new StateManager(this);
//	pStateManager_->AddState(new PlayerWait(pStateManager_));
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

	if (currtime_ > 0) {
		currtime_--;
		if (currtime_ <= 0) flag_ = true;
	}

	if (flag_) {
		flag_ = false;
		pNavigationAI_->Navi(targetPos_);
	}
	else if(currtime_ <= 0){
		XMVECTOR pos = XMLoadFloat3(&transform_.position_);
		XMVECTOR tar = XMLoadFloat3(&targetPos_);
		XMVECTOR vMove = tar - pos;
		vMove = XMVector3Normalize(vMove) * moveSpeed;
		XMStoreFloat3(&transform_.position_, pos + vMove);

		float length = XMVectorGetX(XMVector3Length(tar - pos));
		if (length <= moveSpeed) currtime_ = stopTime_;

	}

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

FeetWait::FeetWait(StateManager* owner)
{
}

void FeetWait::Update()
{
}
