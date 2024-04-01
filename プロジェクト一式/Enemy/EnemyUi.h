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

	int hPict_[MAX];		//画像ハンドル
	int gaugeAlpha_;		//ゲージの透明度
	float height_;			//uiの高さ
	float parcent;			//hpのパーセント
	float foundParcent_;	//FoundのパAlphaーセント
	bool isDraw_;			//表示するかどうか
	Transform transform_[2];//HpとHPBack用のTransform
	EnemyBase* pParent_;	//親

	//Gaugeの透明度計算する
	void SetGaugeAlpha(int value);

public:
	EnemyUi(EnemyBase* parent);
	~EnemyUi();
	void Initialize(float height); //height：Uiの高さ
	void Draw();
	void SetParcent(float f);
	void SetIsDraw(bool b);
	void InitTargetFoundUi();

};

