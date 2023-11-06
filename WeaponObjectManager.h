#pragma once
#include <vector>

class WeaponObject;
class GameManager;
class WeaponBase;

class WeaponObjectManager
{
public:
	enum WEAPON_TYPE {
		WT_SUB1 = 0,
		WT_SUB2,
		WT_MAX,
	};

private:
	float range_;							//�擾�ł���͈�
	WeaponObject* nearestObject_;			//��ԋ߂�WeaponObject
	std::vector<WeaponObject* > objctList_;	//WeaponObject�̃��X�g
	GameManager* pParent_;					//GameManager

public:
	WeaponObjectManager(GameManager* parent);

	void AddWeaponObject(WEAPON_TYPE type);
	void RemoveWeaponObject(WeaponObject* obj);
	void AllKillWeaponObject();

	//nearestObject_�̌v�Z�����āA�͈͓��Ɉ�ł������true
	bool IsInPlayerRange();

	//��ԋ߂�WeaponObject��WeaponBase�̃|�C���^���擾
	WeaponBase* GetNearestWeapon();

};

