#pragma once
#include <vector>
#include <DirectXMath.h>

using namespace DirectX;
class WeaponObject;
class WeaponBase;

class WeaponObjectManager
{
	float range_;							//�擾�ł���͈�
	WeaponObject* nearestObject_;			//��ԋ߂�WeaponObject
	std::vector<WeaponObject* > objctList_;	//WeaponObject�̃��X�g
	
	void CaclNearestObject();

public:
	enum class WEAPON_TYPE {
		WT_SUB1 = 0,
		WT_SUB2,
		WT_MAX,
	};

	WeaponObjectManager();
	~WeaponObjectManager();
	void SceneTransitionInitialize();
	void AddWeaponObject(WEAPON_TYPE type, XMFLOAT3 pos);
	void RemoveWeaponObject(WeaponObject* obj);
	void AllKillWeaponObject();

	//�͈͓���WeaponObject�����邩�ǂ���
	bool IsInPlayerRange();

	//��ԋ߂�WeaponObject��WeaponBase�̃|�C���^���擾
	WeaponBase* GetNearestWeapon();

};

