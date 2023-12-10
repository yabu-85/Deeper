#pragma once
#include "Engine/Transform.h"

class EnemyBase;
class Player;

class EnemyUi
{
	enum PNG_INDEX {
		GAUGE = 0,
		BACK,
		FOUND,
		MAX,
	};

	int hPict_[MAX];
	int gageAlpha_;			//�Q�[�W�̓����x
	float height_;			//ui�̍���
	float parcent;			//hp�̃p�[�Z���g
	float foundParcent_;	//Found�̃pAlpha�[�Z���g
	Transform transform_[2];//Hp��HPBack�p��Transform
	EnemyBase* pParent_;
	Player* pPlayer_;

	void SetGageAlpha(int value);

public:
	EnemyUi(EnemyBase* parent);
	~EnemyUi();
	void Initialize(float height); //height�FUi�̍���
	void Draw();
	void SetParcent(float f);
	void InitTargetFoundUi();

};

