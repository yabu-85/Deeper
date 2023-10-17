#include "Player.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Aim.h"
#include "GameManager.h"
#include "EnemyBase.h"

#include "Engine/Text.h"

namespace {
    float gradually = 0.1f;                 //移動スピードの加減の値
    const float stopGradually = 0.25f;      //移動スピードの加減の値止まるとき
    const float moveGradually = 0.15f;      //移動スピードの加減の値移動時
    const float maxMoveSpeed = 3.0f;        //最大移動スピード

    Text* pText = new Text;

}

Player::Player(GameObject* parent)
    : GameObject(parent, "Player"), hModel_(-1), pAim_(nullptr), playerMovement_{0,0,0}, moveVec_(0,0,0)
{
    vDirection_ = XMVectorZero();
    moveSpeed_ = 0.3f;
}

Player::~Player()
{
}

void Player::Initialize()
{
    //モデルデータのロード
    hModel_ = Model::Load("Model/SphereCollider.fbx");
    assert(hModel_ >= 0);

    transform_.scale_ = { 8.0f, 8.0f, 8.0f };
    transform_.scale_ = { 1.0f, 1.0f, 1.0f };

    pAim_ = Instantiate<Aim>(this);
    
    pText->Initialize();

}

void Player::Update()
{
    CalcMove();         //移動量計算
    CalcDirection();    //方向計算

    if (Input::IsKeyDown(DIK_Q)) {
        pAim_->SetTargetEnemy();
    }

    XMFLOAT3 prePos = transform_.position_;
    if (playerMovement_.x != 0.0f || playerMovement_.z != 0.0f) {

        //当たり判定やりたい
        //下の三角形と円の衝突判定らしい・ゲームには合わないと

        /*        　
        次の順に判定します。
        三角形を含む平面と球の中心との距離が球の半径を越えていたら 非交差と判定します。
        球の中心から三角形を含む平面へ下ろした足を p+t(-n)、 三角形を (1-u-v)q0+uq1+vq2 で表します (t, u, v は媒介変数、n は三角形の法線)。
        Tomas Moller のアルゴリズム
        [n (q1-q0) (q2-q0)]	|t|
        |u|
        |v|
        =	(p-q0)
        を Cramer の公式で解きます。 0.0≦u, 0.0≦v, u+v≦1.0 なら交差と判定します。
        球の中心と三角形の最短距離を求めて、球の半径よりも小さければ交差と判定します。
        最短距離候補として、三角形の3頂点と球の中心との距離の最小値、 もしくは三角形の各辺と球の中心との距離の最小値があります。
        u<0.0, 1.0<v, 1.0<u+v の時 q2 と球の中心との距離
        v<0.0, 1.0<u, 1.0<u+v の時 q1 と球の中心との距離
        u<0.0, v<0.0, u+v<1.0 の時 q0 と球の中心との距離
        が最短距離になります。 いずれでもない時、
        u<0.0 なら線分q0q1 と球の中心との距離、
        v<0.0 なら線分q0q2と球の中心との距離、
        1.0<u+v なら線分q1q2と球の中心との距離
        が最短距離になります。 ただし、この u, v の値の範囲による場合分けは、
        三角形に鈍角が含まれていると破綻するため、ボロノイ領域を厳密に求めるか、 全て求めて最小値を取った方が良いです。
        
        条件分けが多いため、計算効率が悪く、ゲームなどでの使用は非実用的です。
        */

    }

    transform_.position_.x += ((playerMovement_.x * moveSpeed_));
    transform_.position_.z += ((playerMovement_.z * moveSpeed_));

    if (Input::IsKey(DIK_UPARROW)) transform_.position_.y += 0.1f;
    if (Input::IsKey(DIK_DOWNARROW)) transform_.position_.y -= 0.1f;
    if (Input::IsKeyDown(DIK_LEFTARROW)) transform_.position_.y = 0.0f;
    if (Input::IsKeyDown(DIK_RIGHTARROW)) transform_.position_.y += 10.0f;

}

void Player::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_, 2);

    pText->Draw(30, 30, (int)transform_.position_.x);
    pText->Draw(30, 70, (int)transform_.position_.y);
    pText->Draw(30, 110, (int)transform_.position_.z);

}

void Player::Release()
{
}

bool Player::GetMoveVec(XMVECTOR& vec)
{
    vec = XMLoadFloat3(&moveVec_);
    return XMVector3NotEqual(vec, XMVectorZero());
}

void Player::CalcMove()
{
    XMFLOAT3 fMove = { 0,0,0 };
    XMFLOAT3 aimDirection = pAim_->GetAimDirection();
    moveVec_ = { 0,0,0 };
    gradually = stopGradually;

    // PlayerクラスのMove関数内の一部
    if (Input::IsKey(DIK_W)) {
        fMove.x += aimDirection.x;
        fMove.z += aimDirection.z;
        gradually = moveGradually;
    }
    if (Input::IsKey(DIK_A)) {
        fMove.x -= aimDirection.z;
        fMove.z += aimDirection.x;
        gradually = moveGradually;
    }
    if (Input::IsKey(DIK_S)) {
        fMove.x -= aimDirection.x;
        fMove.z -= aimDirection.z;
        gradually = moveGradually;
    }
    if (Input::IsKey(DIK_D)) {
        fMove.x += aimDirection.z;
        fMove.z -= aimDirection.x;
        gradually = moveGradually;
    }

    XMVECTOR vMove = XMLoadFloat3(&fMove);
    vMove = XMVector3Normalize(vMove);
    XMStoreFloat3(&fMove, vMove);
    moveVec_ = fMove;

    fMove = { ((fMove.x - playerMovement_.x) * gradually) , 0.0f , ((fMove.z - playerMovement_.z) * gradually) };
    playerMovement_ = { playerMovement_.x + fMove.x , 0.0f , playerMovement_.z + fMove.z };

    float currentSpeed = XMVectorGetX(XMVector3Length(XMLoadFloat3(&playerMovement_)));

    //MaxSpeed超えていたら正規化・MaxSpeedの値にする
    if (currentSpeed > maxMoveSpeed) {
        XMVECTOR vMove;
        vMove = XMLoadFloat3(&playerMovement_);
        vMove = XMVector3Normalize(vMove);
        vMove *= maxMoveSpeed;

        XMStoreFloat3(&playerMovement_, vMove);
    }

}

void Player::CalcDirection()
{
    XMFLOAT3 fCurePos = XMFLOAT3(transform_.position_.x + playerMovement_.x, 0.0f, transform_.position_.z + playerMovement_.z);
    XMVECTOR curePos = XMLoadFloat3(&fCurePos);

    XMFLOAT3 fPrePos = XMFLOAT3(transform_.position_.x, 0.0f, transform_.position_.z);
    XMVECTOR prePos = XMLoadFloat3(&fPrePos);

    vDirection_ = prePos - curePos;
}
