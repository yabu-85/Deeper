#pragma once
#include "Engine/GameObject.h"
#include <vector>

//プレイヤースキルをなんかやって数値化
//	↓
//動的に難易度を調整する
//	↓
//EnemyControllerとかに難易度を変えるよう指示
//	↓
//色々

class EnemyBase;
class EnemySpawnCtrl;
class NavigationAI;

class GameManager : public GameObject
{
	EnemySpawnCtrl* pEnemySpawnCtrl_;
	NavigationAI* pNavigationAI_;

public:
	GameManager(GameObject* parent);
	~GameManager();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	EnemySpawnCtrl* GetEnemySpawnCtrl() { return pEnemySpawnCtrl_; };
	NavigationAI* GetNavigationAI() { return pNavigationAI_; };

};