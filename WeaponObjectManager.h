#pragma once
#include <vector>
#include <DirectXMath.h>

using namespace DirectX;
class WeaponObject;
class GameManager;
class WeaponBase;

enum WEAPON_TYPE {
	WT_SUB1 = 0,
	WT_SUB2,
	WT_MAX,
};

class WeaponObjectManager
{
	float range_;							//�擾�ł���͈�
	WeaponObject* nearestObject_;			//��ԋ߂�WeaponObject
	std::vector<WeaponObject* > objctList_;	//WeaponObject�̃��X�g
	GameManager* pParent_;					//GameManager

public:
	WeaponObjectManager(GameManager* parent);

	void AddWeaponObject(WEAPON_TYPE type, XMFLOAT3 pos);
	void RemoveWeaponObject(WeaponObject* obj);
	void AllKillWeaponObject();

	//�͈͓��Ɉ�ł������true / nearestObject�̍X�V���S���Ă�
	bool IsInPlayerRange();

	//��ԋ߂�WeaponObject��WeaponBase�̃|�C���^���擾
	WeaponBase* GetNearestWeapon();

};

