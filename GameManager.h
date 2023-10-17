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