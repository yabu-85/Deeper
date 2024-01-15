#pragma once
#include "Engine/GameObject.h"
#include "EnemyBase.h"

class Player;
class CollisionMap;

class Aim : public GameObject
{
    float perspectiveDistance_;     //どのくらい後ろから映すか
    float defPerspectDistance_;     //デフォルトの視点の距離
    float mouseSensitivity;         //マウス感度

    bool isMove_;                   //エイムを動かすかどうか
    bool isCompulsion_;             //強制的に移動させる状態か
    bool isTarget_;                 //ターゲット状態か

    XMFLOAT3 cameraTarget_;         //カメラの焦点目標
    XMFLOAT3 cameraPos_;            //カメラの場所目標
    XMFLOAT3 compulsionTarget_;     //強制時のカメラの焦点目標
    XMFLOAT3 compulsionPos_;        //強制時のカメラの場所目標
    
    XMFLOAT3 aimDirection_;         //現在の視点に基づいた進行方向ベクトル
    XMFLOAT3 plaPos_;               //プレイヤー位置
    XMFLOAT3 cameraOffset_;         //カメラの移動量

    Player* pPlayer_;
    EnemyBase* pEnemyBase_;
    CollisionMap* pCollisionMap_;

    void FacingTarget();
    void CalcCameraOffset(float _aimMove);
    void RayCastStage(XMFLOAT3 _start);

public:
    Aim(GameObject* parent);
    ~Aim();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    void SetCompulsion(bool b) { isCompulsion_ = b; }
    bool IsCompulsion() { return isCompulsion_; }
    void SetAimMove(bool b) { isMove_ = b; };
    bool IsAimMove() { return isMove_; }
    float GetMouseSensitivity() { return mouseSensitivity; };
    void SetMouseSensitivity(float f) { mouseSensitivity = f; };

    //進行方向ベクトルのAim情報を取得
    XMFLOAT3 GetAimDirection() { return aimDirection_; }

    //Targetを見つける
    void SetTargetEnemy();
    bool IsTarget() { return isTarget_; };
    EnemyBase* GetTargetEnemy() { return pEnemyBase_; }
    XMFLOAT3 GetTargetPos() { return isTarget_ ? pEnemyBase_->GetPosition() : XMFLOAT3(); }

    //座標を指定してポジションと焦点を設定する
    void SetCompulsionPosition(XMFLOAT3 pos) { compulsionPos_ = pos; }
    void SetCompulsionTarget(XMFLOAT3 tar) { compulsionTarget_ = tar; }

};