#pragma once
#include <vector>
#include <Windows.h>

class EnemyManager;
class NavigationAI;
class WeaponObjectManager;
class CollisionMap;
class StageBase;
class Player;
class CreateStage;
class SceneManager;

namespace GameManager
{
	void Initialize();
	void Update();
	void Draw();
	void SceneTransitionInitialize();

	//ポーズメニュー閉じる
	void PauseClose();

	//今マウス制限を掛けるべきか
	bool IsMouseLimitedScene();

	EnemyManager* GetEnemyManager();
	NavigationAI* GetNavigationAI();
	WeaponObjectManager* GetWeaponObjectManager();
	CreateStage* GetCreateStage();
	SceneManager* GetSceneManager();
	void SetSceneManager(SceneManager* ma);
	CollisionMap* GetCollisionMap();
	void SetCollisionMap(CollisionMap* map);
	Player* GetPlayer();
	void SetPlayer(Player* player);
	StageBase* GetStage();
	void SetStage(StageBase* stage);
};