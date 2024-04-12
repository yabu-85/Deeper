#pragma once
#include <vector>
#include <DirectXMath.h>
#include "../Enemy/EnemyManager.h"

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
	WeaponObjectManager();
	~WeaponObjectManager();
	void SceneTransitionInitialize();

	//�S�Ă�WeaponObject��Kill
	void AllKillWeaponObject();
	
	//List�ɒǉ�
	void AddWeaponObject(ENEMY_TYPE type, XMFLOAT3 pos);
	//List����폜
	void RemoveWeaponObject(WeaponObject* obj);

	//�͈͓���WeaponObject�����邩�ǂ���
	bool IsInPlayerRange();

	//��ԋ߂�WeaponObject���v�Z���Ă���Weapon���擾
	WeaponBase* GetNearestWeapon();

	//Weapon�S��UI��\�����邩�ǂ���
	void InteractUIIsInPlayerRange();


};

