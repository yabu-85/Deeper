#pragma once
#include "Collider.h"

//-----------------------------------------------------------
//直線の当たり判定
//-----------------------------------------------------------
class LineCollider : public Collider
{
	XMFLOAT3 vec_; //直線の向き
	
	//Colliderクラスのprivateメンバにアクセスできるようにする
	friend class Collider;

	//接触判定
	//引数：target	相手の当たり判定
	//戻値：接触してればtrue
	bool IsHit(Collider* target) override;

public:
	//コンストラクタ（当たり判定の作成）
	//引数：basePos	当たり判定の中心位置（ゲームオブジェクトの原点から見た位置）
	//引数：vec 	当たり判定の向き
	//引数：range	当たり判定のサイズ
	LineCollider(XMFLOAT3 center, XMFLOAT3 vec, float range);

	void SetVec(XMFLOAT3 _vec) { vec_ = _vec; }

};