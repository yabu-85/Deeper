#include "LineCollider.h"
#include "SphereCollider.h"
#include "BoxCollider.h"
#include "Model.h"

//コンストラクタ（当たり判定の作成）
//引数：basePos	当たり判定の中心位置（ゲームオブジェクトの原点から見た位置）
//引数：size	当たり判定のサイズ
LineCollider::LineCollider(XMFLOAT3 center, XMFLOAT3 vec, float range)
{
	center_ = center;
	size_ = XMFLOAT3(range, range, range);
	vec_ = vec;
	type_ = COLLIDER_LINE;

	//リリース時は判定枠は表示しない
#ifdef _DEBUG
	//テスト表示用判定枠
	hDebugModel_ = Model::Load("DebugCollision/sphereCollider.fbx");
#endif
}

//接触判定
//引数：target	相手の当たり判定
//戻値：接触してればtrue
bool LineCollider::IsHit(Collider* target)
{
	if (target->type_ == COLLIDER_BOX)
		return IsHitBoxVsLine((BoxCollider*)target, this);
	else if (target->type_ == COLLIDER_CIRCLE)
		return IsHitCircleVsLine((SphereCollider*)target, this);
	else 
		return IsHitLineVsLine((LineCollider*)target, this);
}