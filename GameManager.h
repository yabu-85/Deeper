#pragma once

class EnemyManager;
class NavigationAI;
class DamageManager;
class WeaponObjectManager;
class DropTable;
class GameObject;

namespace GameManager
{
	void Initialize(GameObject* parent);
	void Update();
	void Release();

	EnemyManager* GetEnemyManager();
	NavigationAI* GetNavigationAI();
	DamageManager* GetDamageManager();
	WeaponObjectManager* GetWeaponObjectManager();
	DropTable* GetDropTable();
	GameObject* GetParent();
};