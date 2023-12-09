#pragma once
#include "Engine/Transform.h"

class EnemyBase;
class Player;

class EnemyUi
{
	int hPict_[2];
	int gageAlpha_;			//ゲージの透明度
	float height_;			//uiの高さ
	float parcent;			//hpのパーセント
	Transform transform_[2];
	EnemyBase* pParent_;
	Player* pPlayer_;

	void SetGageAlpha(int value);

public:
	EnemyUi(EnemyBase* parent);
	~EnemyUi();
	void Initialize(float height); //height：Uiの高さ
	void Draw();
	void SetParcent(float f);

};

