#pragma once
#include "../Engine/GameObject.h"
#include "../Enemy/EnemyBase.h"

class Player;
class CollisionMap;

class Aim : public GameObject
{
    bool isValid_;                  //全ての挙動を止める
    bool isMove_;                   //エイムを動かすかどうか
    bool isCompulsion_;             //強制的に移動させる状態か
    bool isTarget_;                 //ターゲット状態か
    bool isTargetChange_;           //Mouseの移動でターゲットを変えるのに使うフラグ
    
    int hPict_;
    int targetChangeTime_;          //Mouseの移動でターゲットを変えるクールダウン
    int compulsionTime_;            //強制から戻るのに掛かる時間

    int iterations_;            //反復回数
    float sign_;                //計算用CameraShake
    float shakeSpeed_;          //Shakeする速さ
    float range_;               //Shakeする強さ（長さ）
    float moveDistance_;        //計算用rangeの値を入れる
    float distanceDecrease_;    //反復ごとのspeed減衰地
    float rangeDecrease_;       //反復ごとのrange減衰地
    XMVECTOR center_;
    XMVECTOR shakeDirection_;

    float perspectiveDistance_;     //どのくらい後ろから映すか
    float defPerspectDistance_;     //デフォルトの視点の距離
    float mouseSensitivity;         //マウス感度

    XMFLOAT3 cameraTarget_;         //カメラの焦点目標
    XMFLOAT3 cameraPosition_;       //カメラの場所目標
    XMFLOAT3 compulsionTarget_;     //強制時のカメラの焦点目標
    XMFLOAT3 compulsionPosisiton_;  //強制時のカメラの場所目標
    XMFLOAT3 aimDirection_;         //現在の視点に基づいた進行方向ベクトル
    XMFLOAT3 cameraOffset_;         //カメラの移動量

    Player* pPlayer_;
    EnemyBase* pEnemyBase_;
    CollisionMap* pCollisionMap_;

    void DefaultAim();      //通常のAimの計算
    void Compulsion();      //強制移動時の計算
    void BackCompulsion();  //強制移動から通常に移るときの計算

    void FacingTarget();
    void ChangeTarget(XMFLOAT3 mouse);
    void CalcCameraOffset(float _aimMove);
    void RayCastStage();
    void CalcMouseMove();
    XMVECTOR CalcDirection(float x, float y);

    void CameraShake();

public:
    Aim(GameObject* parent);
    ~Aim();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    void SetValid(bool b) { isValid_ = b; }
    bool IsValid() { return isValid_; };
    void SetAimMove(bool b) { isMove_ = b; };
    bool IsAimMove() { return isMove_; }

    float GetMouseSensitivity() { return mouseSensitivity; };
    void SetMouseSensitivity(float f) { mouseSensitivity = f; };

    //進行方向ベクトルのAim情報を取得
    XMFLOAT3 GetAimDirection() { return aimDirection_; }

    //死んだ敵のポインタを渡してTargetだったらの関数
    void TargetIsDead(EnemyBase* target);       
    //Targetを更新する
    void SetTargetEnemy();
    //視野内・近い敵を取得する
    EnemyBase* CalcTargetEnemy();

    EnemyBase* GetTargetEnemy() { return pEnemyBase_; }
    bool IsTarget() { return isTarget_; };
    XMFLOAT3 GetTargetPos() { return isTarget_ ? pEnemyBase_->GetPosition() : XMFLOAT3(); }

    //座標を指定してポジションと焦点を設定する
    void SetCompulsion(XMFLOAT3 pos, XMFLOAT3 tar);
    bool IsCompulsion() { return isCompulsion_; }

    //反復回数、反復する距離、スピードの減衰地、スピード
    void SetCameraShake(int iterat, float range, float range_decrease, float speed, float speed_decrease);
    void SetCameraShakeDirection(XMVECTOR v) { shakeDirection_ = v; }

};