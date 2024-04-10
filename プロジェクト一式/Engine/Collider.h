#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;

//クラスの前方宣言
class GameObject;
class BoxCollider;
class SphereCollider;

//あたり判定のタイプ
enum ColliderType
{
	COLLIDER_BOX,		//箱型
	COLLIDER_CIRCLE,	//球体
	COLLIDER_MAX,		
};

//-----------------------------------------------------------
//あたり判定を管理するクラス
//-----------------------------------------------------------
class Collider
{
	//それぞれのクラスのprivateメンバにアクセスできるようにする
	friend class BoxCollider;
	friend class SphereCollider;

protected:
	GameObject*		pGameObject_;		//この判定をつけたゲームオブジェクト
	ColliderType	type_;				//種類
	XMFLOAT3		center_;			//中心位置（ゲームオブジェクトの原点から見た位置）
	XMFLOAT3		size_;				//判定サイズ（幅、高さ、奥行き）
	int				hDebugModel_;		//デバッグ表示用のモデルのID
	bool			isValid_;			//判定するかどうか

public:
	//コンストラクタ
	Collider();

	//デストラクタ
	virtual ~Collider();

	//接触判定（継承先のSphereColliderかBoxColliderでオーバーライド）
	//引数：target	相手の当たり判定
	//戻値：接触してればtrue
	virtual bool IsHit(Collider* target) = 0;

	void SetValid(bool b) { isValid_ = b; }
	bool IsValid() { return isValid_; }

	//-------------------------------------------------------

	//箱型同士の衝突判定
	//引数：boxA	１つ目の箱型判定
	//引数：boxB	２つ目の箱型判定
	//戻値：接触していればtrue
	bool IsHitBoxVsBox(BoxCollider* boxA, BoxCollider* boxB);

	//箱型と球体の衝突判定
	//引数：box	    1つ目の箱型判定
	//引数：sphere  ２つ目の線分判定
	//戻値：接触していればtrue
	bool IsHitBoxVsCircle(BoxCollider* box, SphereCollider* sphere);

	//-------------------------------------------------------

	//球体同士の衝突判定
	//引数：circleA	１つ目の球体判定
	//引数：circleB	２つ目の球体判定
	//戻値：接触していればtrue
	bool IsHitCircleVsCircle(SphereCollider* circleA, SphereCollider* circleB);

	//-------------------------------------------------------

	//テスト表示用の枠を描画
	//引数：position	オブジェクトの位置
	void Draw(XMFLOAT3 position);

	void SetGameObject(GameObject* gameObject) { pGameObject_ = gameObject; }
	GameObject* GetGameObject() { return pGameObject_; }

	XMFLOAT3 GetCenter() { return center_; };
	void SetCenter(XMFLOAT3 pos) { center_ = pos; }

	void SetSize(float _size) { size_ = XMFLOAT3(_size, _size, _size); }
	XMFLOAT3 GetSize() { return size_; }
};

