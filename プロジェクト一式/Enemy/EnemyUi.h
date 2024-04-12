#pragma once
#include "../Engine/Transform.h"

class EnemyBase;

class EnemyUi
{
	enum PNG_INDEX {
		GAUGE = 0,
		BACK,
		FOUND,
		MAX,
	};

	bool isBossGauge_;

	int hPict_[MAX];		//�摜�n���h��
	int gaugeAlpha_;		//�Q�[�W�̓����x
	float height_;			//ui�̍���
	float parcent;			//hp�̃p�[�Z���g
	float foundParcent_;	//Found�̃pAlpha�[�Z���g
	bool isDraw_;			//�\�����邩�ǂ���
	Transform transform_[2];//Hp��HPBack�p��Transform
	EnemyBase* pParent_;	//�e

	//Gauge�̓����x�v�Z����
	void SetGaugeAlpha(int value);

public:
	EnemyUi(EnemyBase* parent);
	~EnemyUi();
	void Initialize(float height); //height�FUi�̍���
	void Draw();
	void SetParcent(float f);
	void SetIsDraw(bool b);
	void InitTargetFoundUi();

	void SetBssGauge() { isBossGauge_ = true; }
	void SetAlphaMax();
};

