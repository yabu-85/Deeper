#include "WeaponBase.h"
#include "../State/StateManager.h"

WeaponBase::WeaponBase(GameObject* parent, std::string name)
	: GameObject(parent, name), hModel_(-1), pStateManager_(nullptr), isAtkEnd_(true), isCancellable_(false), isReadyNext_(false), durance_(0),
	boneIndex_(-1), partIndex_(-1), type_(ENEMY_TYPE::ENEMY_STONEGOLEM)
{
}

void WeaponBase::UpdateState()
{
	pStateManager_->Update();
}

bool WeaponBase::IsBlockend()
{
    if (durance_ <= 0) return true;
    return false;
}
