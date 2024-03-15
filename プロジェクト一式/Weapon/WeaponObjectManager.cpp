#include "WeaponObjectManager.h"
#include "WeaponObject.h"
#include "NormalGunWeapon.h"
#include "StoneArmWeapon.h"
#include "../Engine/Model.h"
#include "../Engine/Global.h"
#include "../Player/Player.h"
#include "../GameManager/GameManager.h"
#include "../UI/InteractionUI.h"
#include "../Scene/StageBase.h"

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
	std::string fileName[(int)WEAPON_TYPE::WT_MAX] = { "StoneArmObj", "RedBoxObj", "RedBoxObj"};

	WeaponObject* weapon = InstantiateFront<WeaponObject>(GameManager::GetStage());
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
		if (DistanceCalculation(plaPos, objctList_.at(i)->GetPosition()) <= range_) {
			return true;
		}
	}

	return false;
}

WeaponBase* WeaponObjectManager::GetNearestWeapon()
{
	CaclNearestObject();
	if (nearestObject_ == nullptr) return nullptr;

	WeaponBase* weapon = nullptr;
	if (nearestObject_->GetType() == (int)WEAPON_TYPE::WT_STONE) weapon = Instantiate<StoneArmWeapon>(GameManager::GetPlayer());
	else if (nearestObject_->GetType() == (int)WEAPON_TYPE::WT_THROW) weapon = Instantiate<NormalGunWeapon>(GameManager::GetPlayer());
	else if (nearestObject_->GetType() == (int)WEAPON_TYPE::WT_MELEE) weapon = Instantiate<NormalGunWeapon>(GameManager::GetPlayer());
	
	//�폜
	RemoveWeaponObject(nearestObject_);
	nearestObject_->KillMe();
	nearestObject_ = nullptr;

	return weapon;
}

void WeaponObjectManager::CaclNearestObject()
{
	XMFLOAT3 plaPos = GameManager::GetPlayer()->GetPosition();
	int minRangeIndex = -1;
	float minRange = range_;
	nearestObject_ = nullptr;

	//��ԋ߂��I�u�W�F�N�g�̃C���f�b�N�X�v�Z
	for (int i = 0; i < objctList_.size(); i++) {
		float range = DistanceCalculation(plaPos, objctList_.at(i)->GetPosition());
		if (minRange > range) {
			minRangeIndex = i;
			minRange = range;
		}
	}

	//�͈͓��ɂ���΍X�V
	if (minRangeIndex >= 0) {
		nearestObject_ = objctList_.at(minRangeIndex);
	}
}

void WeaponObjectManager::InteractUIIsInPlayerRange()
{
	//range�͈͓̔��Ȃ�InteractionUI��Valid��true��
	XMFLOAT3 plaPos = GameManager::GetPlayer()->GetPosition();
	for (int i = 0; i < objctList_.size(); i++) {
		if(DistanceCalculation(plaPos, objctList_.at(i)->GetPosition()) <= range_) objctList_.at(i)->GetInteractionUI()->SetValid(true);
		else objctList_.at(i)->GetInteractionUI()->SetValid(false);
	}
}
