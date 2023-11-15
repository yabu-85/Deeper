#pragma once
#include "Engine/Transform.h"
#include "Engine/VFX.h"

class EnemyBase;
class Aim;

class EnemyUi
{
	int vHandle_[2];
	float height_;			//uiの高さ
	float parcent;			//hpのパーセント
	XMFLOAT2 size;			//UiのHpに合わせたサイズ
	Transform transform_;
	EnemyBase* pParent_;
	EmitterData data_;

public:
	EnemyUi(EnemyBase* parent);
	~EnemyUi();
	void Initialize(float height); //height：Uiの高さ
	void Update();

	void SetParcent(float f) { parcent = f; };

};

