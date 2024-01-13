#pragma once

class EnemyManager;
class NavigationAI;
class WeaponObjectManager;
class DropTable;
class CollisionMap;
class GameObject;
class Player;
class CreateStage;

namespace GameManager
{
	void Initialize();
	void Update();
	void Release();
	void Draw();

	EnemyManager* GetEnemyManager();
	NavigationAI* GetNavigationAI();
	WeaponObjectManager* GetWeaponObjectManager();
	DropTable* GetDropTable();
	GameObject* GetParent();
	CreateStage* GetCreateStage();

	void SetCollisionMap(CollisionMap* map);
	CollisionMap* GetCollisionMap();

	void SetPlayer(Player* player);
	Player* GetPlayer();

	void SetStage(GameObject* stage);
	GameObject* GetStage();

};