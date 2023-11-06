#include "WeaponObjectManager.h"
#include "WeaponObject.h"
#include "Engine/Model.h"
#include "Player.h"
#include "TestWeaponSub.h"
#include "GameManager.h"

WeaponObjectManager::WeaponObjectManager(GameManager* parent)
	: pParent_(parent), range_(0), nearestObject_(nullptr)
{
	range_ = 3.0f;
	AddWeaponObject(WT_SUB1);
	AddWeaponObject(WT_SUB1);
	AddWeaponObject(WT_SUB2);
	AddWeaponObject(WT_SUB2);
}

void WeaponObjectManager::AddWeaponObject(WEAPON_TYPE type)
{
	std::string fileName[WT_MAX] = { "Hand", "RedBox" };

	WeaponObject* weapon = Instantiate<WeaponObject>(pParent_);
	weapon->LoadModel(fileName[type]);
	weapon->SetType(type);
	objctList_.push_back(weapon);
}

void WeaponObjectManager::RemoveWeaponObject(WeaponObject* obj)
{
	for (auto it = objctList_.begin(); it != objctList_.end();) {
		if (*it == obj) {
			it = objctList_.erase(it);
			return;
		}
		else {
			it++;
		}
	}
}

void WeaponObjectManager::AllKillWeaponObject()
{
	for (WeaponObject* obj : objctList_) {
		obj->KillMe();
	}
	objctList_.clear();
	nearestObject_ = nullptr;
}

bool WeaponObjectManager::IsInPlayerRange()
{
	Player* pPlayer = (Player*)pParent_->FindObject("Player");
    XMFLOAT3 plaPos = pPlayer->GetPosition();
	
	int minRangeIndex = -1;
	float minRange = range_;
	
	for (int i = 0; i < objctList_.size(); i++) {
		XMFLOAT3 objPos = objctList_.at(i)->GetPosition();
		objPos = XMFLOAT3(plaPos.x - objPos.x, plaPos.y - objPos.y, plaPos.z - objPos.z);

		float currentSpeed = XMVectorGetX(XMVector3Length(XMLoadFloat3(&objPos)));
		if (minRange > currentSpeed) {
			minRangeIndex = i;
			minRange = currentSpeed;
		}
	}

	if (minRangeIndex >= 0) {
		nearestObject_ = objctList_.at(minRangeIndex);
		return true;
	}

	return false;
}

WeaponBase* WeaponObjectManager::GetNearestWeapon()
{
	if (nearestObject_->GetType() == WT_SUB1) {
		RemoveWeaponObject(nearestObject_);
		nearestObject_->KillMe();
		nearestObject_ = nullptr;
		return Instantiate<TestWeaponSub>((Player*)pParent_->FindObject("Player"));
	}

	if (nearestObject_->GetType() == WT_SUB2) {
		RemoveWeaponObject(nearestObject_);
		nearestObject_->KillMe();
		nearestObject_ = nullptr;
		return nullptr;
	}

	return nullptr;
}
