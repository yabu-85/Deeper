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

