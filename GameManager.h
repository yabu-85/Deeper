#pragma once
#include <vector>

class EnemyManager;
class NavigationAI;
class WeaponObjectManager;
class CollisionMap;
class GameObject;
class Character;
class Player;
class CreateStage;
class SceneManager;

namespace GameManager
{
	void Initialize();
	void Update();
	void Release();
	void Draw();
	void SceneTransitionInitialize();

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

	GameObject* GetStage();
	void SetStage(GameObject* stage);

	std::vector<Character*> GetCharacterList();
	void AddCharacter(Character* c);
	void RemoveCharacter(Character* c);
	
};