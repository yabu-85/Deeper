#include "BoxCollider.h"
#include "SphereCollider.h"
#include "LineCollider.h"
#include "GameObject.h"
#include "Model.h"
#include "Transform.h"

//コンストラクタ
Collider::Collider():
	pGameObject_(nullptr), center_(0.0f, 0.0f, 0.0f), hDebugModel_(-1), size_(0.0f, 0.0f, 0.0f), type_(COLLIDER_MAX)
{
}

//デストラクタ
Collider::~Collider()
{
}

//箱型同士の衝突判定
//引数：boxA	１つ目の箱型判定
//引数：boxB	２つ目の箱型判定
//戻値：接触していればtrue
bool Collider::IsHitBoxVsBox(BoxCollider* boxA, BoxCollider* boxB)
{

	XMFLOAT3 boxPosA = Transform::Float3Add(boxA->pGameObject_->GetWorldPosition(), boxA->center_);
	XMFLOAT3 boxPosB = Transform::Float3Add(boxB->pGameObject_->GetWorldPosition(), boxB->center_);

	if ((boxPosA.x + boxA->size_.x / 2) > (boxPosB.x - boxB->size_.x / 2) &&
		(boxPosA.x - boxA->size_.x / 2) < (boxPosB.x + boxB->size_.x / 2) &&
		(boxPosA.y + boxA->size_.y / 2) > (boxPosB.y - boxB->size_.y / 2) &&
		(boxPosA.y - boxA->size_.y / 2) < (boxPosB.y + boxB->size_.y / 2) &&
		(boxPosA.z + boxA->size_.z / 2) > (boxPosB.z - boxB->size_.z / 2) &&
		(boxPosA.z - boxA->size_.z / 2) < (boxPosB.z + boxB->size_.z / 2))
	{
		return true;
	}
	return false;
}

//箱型と球体の衝突判定
//引数：box	箱型判定
//引数：sphere	２つ目の箱型判定
//戻値：接触していればtrue
bool Collider::IsHitBoxVsCircle(BoxCollider* box, SphereCollider* sphere)
{
	XMFLOAT3 circlePos = Transform::Float3Add(sphere->pGameObject_->GetWorldPosition(), sphere->center_);
	XMFLOAT3 boxPos = Transform::Float3Add(box->pGameObject_->GetWorldPosition(), box->center_);

	if (circlePos.x > boxPos.x - box->size_.x - sphere->size_.x &&
		circlePos.x < boxPos.x + box->size_.x + sphere->size_.x &&
		circlePos.y > boxPos.y - box->size_.y - sphere->size_.x &&
		circlePos.y < boxPos.y + box->size_.y + sphere->size_.x &&
		circlePos.z > boxPos.z - box->size_.z - sphere->size_.x &&
		circlePos.z < boxPos.z + box->size_.z + sphere->size_.x )
	{
		return true;
	}

	return false;
}

//箱型と直線の衝突判定
//引数：box	箱型判定
//引数：line	２つ目の直線判定
//戻値：接触していればtrue
bool Collider::IsHitBoxVsLine(BoxCollider* box, LineCollider* line)
{

	return false;
}

//球体同士の衝突判定
//引数：circleA	１つ目の球体判定
//引数：circleB	２つ目の球体判定
//戻値：接触していればtrue
bool Collider::IsHitCircleVsCircle(SphereCollider* circleA, SphereCollider* circleB)
{
	XMFLOAT3 centerA = circleA->center_;
	XMFLOAT3 positionA = circleA->pGameObject_->GetWorldPosition();
	XMFLOAT3 centerB = circleB->center_;
	XMFLOAT3 positionB = circleB->pGameObject_->GetWorldPosition();

	XMVECTOR v = (XMLoadFloat3(&centerA) + XMLoadFloat3(&positionA))
				- (XMLoadFloat3(&centerB) + XMLoadFloat3(&positionB));

	if (XMVector3Length(v).m128_f32[0] <= circleA->size_.x + circleB->size_.x)
	{
		return true;
	}

	return false;
}

//球体と直線の衝突判定
//引数：circle	１つ目の球体判定
//引数：line	２つ目の直線判定
//戻値：接触していればtrue
bool Collider::IsHitCircleVsLine(SphereCollider* circle, LineCollider* line)
{
	XMFLOAT3 p = circle->center_; //中心点の座標
	p.x += circle->pGameObject_->GetWorldPosition().x;
	p.y += circle->pGameObject_->GetWorldPosition().y;
	p.z += circle->pGameObject_->GetWorldPosition().z;

	XMFLOAT3 l = line->center_;
	XMFLOAT3 linePos = line->pGameObject_->GetWorldPosition();
	l.x += linePos.x;
	l.y += linePos.y;
	l.z += linePos.z;

	float r = circle->size_.x;

	//これStartがPの半径内に入っていたら無条件で当たってるとみなす
	double distSquared = pow(p.x - l.x, 2) + pow(p.y - l.y, 2) + pow(p.z - l.z, 2);
	double radSquared = pow(r, 2);
	if (distSquared < radSquared) {
		return true;
	}

	p.x = p.x - l.x;
	p.y = p.y - l.y;
	p.z = p.z - l.z;

	float A = line->vec_.x * line->vec_.x + line->vec_.y * line->vec_.y + line->vec_.z * line->vec_.z;
	float B = line->vec_.x * p.x + line->vec_.y * p.y + line->vec_.z * p.z;
	float C = p.x * p.x + p.y * p.y + p.z * p.z - r * r;

	if (A == 0.0f)
		return false;  //レイの長さが0

	float s = B * B - A * C;
	if (s < 0.0f) return false; // 衝突していない

	s = sqrtf(s);
	float a1 = (B - s) / A;
	float a2 = (B + s) / A;

	if (a1 < 0.0f || a2 < 0.0f) return false; // レイの反対で衝突

	XMFLOAT3 q1; //衝突開始地点
	q1.x = l.x + a1 * line->vec_.x;
	q1.y = l.y + a1 * line->vec_.y;
	q1.z = l.z + a1 * line->vec_.z;

	//XMFLOAT3 q2; //終了地点
	//q2.x = start.x + a2 * vec.x;
	//q2.y = start.y + a2 * vec.y;
	//q2.z = start.z + a2 * vec.z;

	// ベクトルの差分を計算
	XMVECTOR difference = XMVectorSubtract(XMLoadFloat3(&l), XMLoadFloat3(&q1));
	// ベクトルの長さ（距離）を計算
	XMVECTOR distance = XMVector3Length(difference);
	// 結果を float に変換して返す
	float distanceFloat;
	XMStoreFloat(&distanceFloat, distance);

	//rangeの範囲外だった場合次
	if (line->size_.x < distanceFloat) return false;

	return true;

}

//直線同士の衝突判定
//引数：lineA	１つ目の直線判定
//引数：lineB	２つ目の直線判定
//戻値：接触していればtrue
bool Collider::IsHitLineVsLine(LineCollider* lineA, LineCollider* lineB)
{
	return false;
}

//テスト表示用の枠を描画
//引数：position	オブジェクトの位置
void Collider::Draw(XMFLOAT3 position)
{
	Transform transform;
	transform.position_ = XMFLOAT3(position.x + center_.x, position.y + center_.y, position.z + center_.z);
	transform.scale_ = size_;
	transform.Calclation();
	Model::SetTransform(hDebugModel_, transform);
	Model::Draw(hDebugModel_, 4);
}
