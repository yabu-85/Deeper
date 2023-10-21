#include "Feet.h"
#include "Engine/Model.h"
#include "NavigationAI.h"
#include "GameManager.h"

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
}

void Feet::Initialize()
{
	//モデルデータのロード
	hModel_ = Model::Load("Model/Feet.fbx");
	assert(hModel_ >= 0);

	transform_.rotate_.y = -90;
	transform_.position_ = XMFLOAT3(50.0f, 0.0f, 50.0f);
	targetPos_ = XMFLOAT3(50.0f, 0.0f, 50.0f);

	GameManager* pGameManager = (GameManager*)FindObject("GameManager");
	pNavigationAI_ = pGameManager->GetNavigationAI();

}

void Feet::Update()
{
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

}

void Feet::Release()
{
}
