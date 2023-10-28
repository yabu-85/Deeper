#pragma once
#include "Engine/GameObject.h"
#include <vector>

//�v���C���[�X�L�����Ȃ񂩂���Đ��l��
//	��
//���I�ɓ�Փx�𒲐�����
//	��
//EnemyController�Ƃ��ɓ�Փx��ς���悤�w��
//	��
//�F�X

class Player;
class Stage;
class EnemyBase;
class EnemySpawnCtrl;
class NavigationAI;
class DamageCtrl;

class GameManager : public GameObject
{
	Player* pPlayer_;
	Stage* pStage_;
	EnemySpawnCtrl* pEnemySpawnCtrl_;
	NavigationAI* pNavigationAI_;
	DamageCtrl* pDamageCtrl_;

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