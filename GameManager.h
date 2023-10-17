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
class GameObject;

class GameManager : public GameObject
{
	std::vector<EnemyBase*> enemyList_;

public:
	GameManager(GameObject* parent);
	~GameManager();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	std::vector<EnemyBase*>& GetAllEnemy() { return enemyList_; };

};

