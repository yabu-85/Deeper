#pragma once

class EnemySpawnCtrl;
class NavigationAI;
class DamageCtrl;
class WeaponObjectManager;
class DropTable;
class GameObject;

namespace GameManager
{
	void Initialize(GameObject* parent);
	void Update();
	void Release();

	EnemySpawnCtrl* GetEnemySpawnCtrl();
	NavigationAI* GetNavigationAI();
	DamageCtrl* GetDamageCtrl();
	WeaponObjectManager* GetWeaponObjectManager();
	DropTable* GetDropTable();
	GameObject* GetParent();
};