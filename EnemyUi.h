#pragma once
#include "Engine/Transform.h"

class EnemyBase;
class Player;

class EnemyUi
{
	int hPict_[2];
	int gageAlpha_;			//�Q�[�W�̓����x
	float height_;			//ui�̍���
	float parcent;			//hp�̃p�[�Z���g
	Transform transform_[2];
	EnemyBase* pParent_;
	Player* pPlayer_;

	void SetGageAlpha(int value);

public:
	EnemyUi(EnemyBase* parent);
	~EnemyUi();
	void Initialize(float height); //height�FUi�̍���
	void Draw();
	void SetParcent(float f);

};

