#include "WeaponObjectManager.h"
#include "../Engine/Model.h"
#include "../Player/Player.h"
#include "../GameManager.h"
#include "WeaponObject.h"
#include "TestWeaponSub.h"
#include "StoneArmWeapon.h"

WeaponObjectManager::WeaponObjectManager()
	: range_(0), nearestObject_(nullptr)
{
	range_ = 1.0f;
}

WeaponObjectManager::~WeaponObjectManager()
{
}

void WeaponObjectManager::SceneTransitionInitialize()
{
	objctList_.clear();
	nearestObject_ = nullptr;

}

void WeaponObjectManager::AddWeaponObject(WEAPON_TYPE type, XMFLOAT3 pos)
{
	std::string fileName[(int)WEAPON_TYPE::WT_MAX] = { "StoneArmObj", "RedBox" };

	WeaponObject* weapon = InstantiateFront<WeaponObject>(GameManager::GetStage());

	//デバッグ用
	if ((int)type == 0) weapon->SetScale(0.2f, 0.2f, 0.2f);

	weapon->SetPosition(pos);
	weapon->LoadModel(fileName[(int)type]);
	weapon->SetType(int(type));
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
	XMFLOAT3 plaPos = GameManager::GetPlayer()->GetPosition();

	for (int i = 0; i < objctList_.size(); i++) {
		XMFLOAT3 objPos = objctList_.at(i)->GetPosition();
		objPos = XMFLOAT3(plaPos.x - objPos.x, plaPos.y - objPos.y, plaPos.z - objPos.z);

		float range = XMVectorGetX(XMVector3Length(XMLoadFloat3(&objPos)));
		if (range <= range_) {
			return true;
		}
	}

	return false;
}

WeaponObject* WeaponObjectManager::GetNearestWeapon()
{
	CaclNearestObject();

	return nearestObject_;
}

WeaponBase* WeaponObjectManager::PlayerWeaponSet()
{
	CaclNearestObject();

	if (nearestObject_ == nullptr) {
		OutputDebugString("nearestObject->Is NullPtr Deth");
		return nullptr;
	}

	if (nearestObject_->GetType() == (int)WEAPON_TYPE::WT_STONE) {
		RemoveWeaponObject(nearestObject_);
		nearestObject_->KillMe();
		nearestObject_ = nullptr;
		return Instantiate<StoneArmWeapon>(GameManager::GetPlayer());
	}

	if (nearestObject_->GetType() == (int)WEAPON_TYPE::WT_SUB) {
		RemoveWeaponObject(nearestObject_);
		nearestObject_->KillMe();
		nearestObject_ = nullptr;
		return Instantiate<TestWeaponSub>(GameManager::GetPlayer());
	}

	return nullptr;
}

float WeaponObjectManager::GetNearestDistance()
{
	Player* pPlayer = GameManager::GetPlayer();
	XMFLOAT3 plaPos = pPlayer->GetPosition();

	float min = range_;
	bool in = false;

	for (int i = 0; i < objctList_.size(); i++) {
		XMFLOAT3 objPos = objctList_.at(i)->GetPosition();
		objPos = XMFLOAT3(plaPos.x - objPos.x, plaPos.y - objPos.y, plaPos.z - objPos.z);

		float len = XMVectorGetX(XMVector3Length(XMLoadFloat3(&objPos)));
		if (min > len) {
			min = len;
			in = true;
		}
	}

	if (in) return min;
	return FBXSDK_FLOAT_MAX;

}

void WeaponObjectManager::CaclNearestObject()
{
	Player* pPlayer = GameManager::GetPlayer();
	XMFLOAT3 plaPos = pPlayer->GetPosition();

	int minRangeIndex = -1;
	float minRange = range_;

	for (int i = 0; i < objctList_.size(); i++) {
		XMFLOAT3 objPos = objctList_.at(i)->GetPosition();
		objPos = XMFLOAT3(plaPos.x - objPos.x, plaPos.y - objPos.y, plaPos.z - objPos.z);

		float range = XMVectorGetX(XMVector3Length(XMLoadFloat3(&objPos)));
		if (minRange > range) {
			minRangeIndex = i;
			minRange = range;
		}
	}

	nearestObject_ = nullptr;
	if (minRangeIndex >= 0) {
		nearestObject_ = objctList_.at(minRangeIndex);
	}

}

